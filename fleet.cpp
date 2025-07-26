// UMBC - CMSC 341 - Spring 2025 - Proj2
#include "fleet.h"

// Default constructor
// Initializes an empty fleet with no specific tree type
Fleet::Fleet() : m_root(nullptr), m_type(NONE){
}

// Overloaded constructor
// Initializes an empty fleet with the given tree type (BST/AVL/SPLAY)
Fleet::Fleet(TREETYPE type) : m_root(nullptr), m_type(type){

}

// Destructor
// Clears the tree to deallocate memory
Fleet::~Fleet(){
    clear();
}

// Deallocates all memory associated with the current fleet tree
void Fleet::clear(){
    clearHelper(m_root);
    m_root = nullptr;
}

// Inserts a new Ship into the Fleet based on the type (BST, AVL, or SPLAY)
void Fleet::insert(const Ship& ship){
    // Skip insertion if fleet type is NONE or ID out of range
    if (m_type == NONE || ship.getID() < MINID || ship.getID() > MAXID)
        return;

    Ship* temp = m_root;
    Ship* prev = nullptr;

    // Locate the position for inserting
    while (temp != nullptr) {
        prev = temp;
        // No Duplicate IDs
        if (temp->getID() == ship.getID())
            return;

        else if (temp->getID() > ship.getID())
            temp = temp->getLeft();

        else
            temp = temp->getRight();
    }

    // Create and link the new Ship node
    Ship* newShip = new Ship(ship.getID(), ship.getType(), ship.getState());

    if (prev == nullptr)
        m_root = newShip;

    else {
        // Attach to left or right based on value
        if (prev->getID() > ship.getID())
            prev->setLeft(newShip);
        else
            prev->setRight(newShip);
    }

    // Update the heights of all the nodes and balance or splay the fleet based on tree type
    updateHeights(m_root);

    if (m_type == AVL)
        rotationAVL(); // Ensure AVL balancing

    else if (m_type == SPLAY)
        rotationSPLAY(newShip); // Perform Splay operation (should splay new node to root)
}

// Deletes a ship by ID if BST or AVL; skips if SPLAY.
// Re-balances if AVL
void Fleet::remove(int id){
    // Skip deletion if fleet type is NONE or SPLAY or ID out of range or the m_root is nullptr
    if(m_root != nullptr && m_type != NONE && m_type != SPLAY) {
        if (id < MINID || id > MAXID)
            return;

        Ship* temp = m_root;
        Ship* prev = nullptr;

        // Searches for the node to remove
        while (temp != nullptr && temp->getID() != id) {
            prev = temp;

            if (temp->getID() > id)
                temp = temp->getLeft();

            else
                temp = temp->getRight();
        }

        if (temp != nullptr) {
            // Case 1: Leaf node
            if (temp->getLeft() == nullptr && temp->getRight() == nullptr) {
                if (prev == nullptr)
                    m_root = nullptr;

                else if (prev->getLeft() == temp)
                    prev->setLeft(nullptr);

                else
                    prev->setRight(nullptr);
            }
            // Case 2: One child
            else if (temp->getLeft() == nullptr || temp->getRight() == nullptr) {
                Ship* child = (temp->getLeft() != nullptr) ? temp->getLeft() : temp->getRight();
                if (prev == nullptr)
                    m_root = child;

                else if (prev->getLeft() == temp)
                    prev->setLeft(child);

                else
                    prev->setRight(child);
            }

            // Case 3: Two children — replace using predecessor or successor
            else {
                Ship* replacement;
                Ship* replacementParent = temp;

                // Decide replacement by comparing subtree heights
                if (temp->getLeft()->getHeight() < temp->getRight()->getHeight()) {
                    // Use in-order predecessor (rightmost node of left subtree)
                    replacement = temp->getLeft();
                    while (replacement->getRight() != nullptr) {
                        replacementParent = replacement;
                        replacement = replacement->getRight();
                    }

                    // If predecessor is not the direct left child
                    if (replacementParent != temp) {
                        replacementParent->setRight(replacement->getLeft());
                        replacement->setLeft(temp->getLeft());
                    }

                    replacement->setRight(temp->getRight());
                }
                else{
                    // Use in-order successor (leftmost node of right subtree)
                    replacement = temp->getRight();
                    while (replacement->getLeft() != nullptr) {
                        replacementParent = replacement;
                        replacement = replacement->getLeft();
                    }

                    // If successor is not the direct right child
                    if (replacementParent != temp) {
                        replacementParent->setLeft(replacement->getRight());
                        replacement->setRight(temp->getRight());
                    }

                    replacement->setLeft(temp->getLeft());
                }

                if (prev == nullptr)
                    m_root = replacement;

                else if (prev->getLeft() == temp)
                    prev->setLeft(replacement);

                else
                    prev->setRight(replacement);
            }

            temp->setLeft(nullptr);
            temp->setRight(nullptr);
            delete temp;

        // Balance the tree if AVL and update heights if BST/AVL
            updateHeights(m_root);
            if (m_type == AVL)
                rotationAVL(); // Ensure AVL balancing

        }
    }
}

// Overloaded Assignment Operator
// Deep copies another fleet object
const Fleet & Fleet::operator=(const Fleet & rhs){
    if (this != &rhs) {
        // Deallocate existing memory
        clear();
        m_type = rhs.m_type;

        Ship* temp = rhs.m_root;

        if (temp != nullptr)
            // Deep copy the tree
            m_root = assignmentOperatorHelper(temp);
    }
    updateHeights(m_root);
    dumpTree();
    return *this;
}

// Returns current tree type
TREETYPE Fleet::getType() const{
    return m_type;
}

// Changes tree type and re-balances if necessary
void Fleet::setType(TREETYPE type){
    if (type == NONE) {
        m_type = type;
        clear(); // Clear the tree if the type is set to NONE
    }

    else if (type == AVL) {
        m_type = type;
        rotationAVL();
    }

    else
     m_type = type;
}

// Detects for an imbalance in AVL tree by checking balance factor
Ship* Fleet::imbalance(Ship *node) {
    if (node == nullptr)
        return nullptr;

    // Calculate the balance factor
    int balanceFactor = (node->getLeft() != nullptr ? node->getLeft()->getHeight() : -1) -
                        (node->getRight() != nullptr ? node->getRight()->getHeight() : -1);

    // If this node is unbalanced, return it
    if (abs(balanceFactor) > 1)
        return node;

    // Check left and right subtrees for imbalance
    Ship* leftImbalance = imbalance(node->getLeft());
    if (leftImbalance != nullptr)
        return leftImbalance;

    Ship* rightImbalance = imbalance(node->getRight());
    if (rightImbalance != nullptr)
        return rightImbalance;

    return nullptr;
}

// Recursively updates heights of all nodes
void Fleet::updateHeights(Ship *node){
    if (node != nullptr) {

        updateHeights(node->getLeft());
        updateHeights(node->getRight());

        int leftHeight = (node->getLeft() != nullptr ? node->getLeft()->getHeight() : -1);
        int rightHeight = (node->getRight() != nullptr ? node->getRight()->getHeight() : -1);

        node->setHeight((leftHeight > rightHeight ? leftHeight : rightHeight) + 1);
    }
}

// Recursive helper to delete all nodes
void Fleet::clearHelper(Ship *root) {
    if (root != nullptr) {
        clearHelper(root->getLeft());
        clearHelper(root->getRight());
        delete root;
    }
}

// Performs AVL rotations (LL, RR, LR, RL) until balanced
void Fleet::rotationAVL() {
    do {
        Ship* node = imbalance(m_root);
        if (node == nullptr)
            return;

        int balanceFactor = (node->getLeft() != nullptr ? node->getLeft()->getHeight() : -1) -
                            (node->getRight() != nullptr ? node->getRight()->getHeight() : -1);

        Ship* parent = nullptr;
        Ship* current = m_root;
        while (current != nullptr && current != node) {
            parent = current;
            current = (node->getID() < current->getID()) ? current->getLeft() : current->getRight();
        }

        // Left Heavy
        if (balanceFactor > 1) {
            Ship* leftChild = node->getLeft();

            if (leftChild != nullptr && leftChild->getRight() != nullptr &&
                leftChild->getRight()->getHeight() >
                ((leftChild->getLeft() != nullptr) ? leftChild->getLeft()->getHeight() : -1)) {
                // Left-Right Rotation
                Ship* leftRightChild = leftChild->getRight();
                leftChild->setRight(leftRightChild->getLeft());
                leftRightChild->setLeft(leftChild);
                node->setLeft(leftRightChild);
            }

            // Left-Left Rotation
            Ship* leftChildNow = node->getLeft();
            node->setLeft(leftChildNow->getRight());
            leftChildNow->setRight(node);

            if (parent == nullptr)
                m_root = leftChildNow;

            else if (parent->getLeft() == node)
                parent->setLeft(leftChildNow);

            else
                parent->setRight(leftChildNow);
        }
        // Right Heavy
        else if (balanceFactor < -1) {
            Ship* rightChild = node->getRight();
            if (rightChild != nullptr && rightChild->getLeft() != nullptr &&
                rightChild->getLeft()->getHeight() >
                ((rightChild->getRight() != nullptr) ? rightChild->getRight()->getHeight() : -1)) {
                // Right-Left Rotation
                Ship* rightLeftChild = rightChild->getLeft();
                rightChild->setLeft(rightLeftChild->getRight());
                rightLeftChild->setRight(rightChild);
                node->setRight(rightLeftChild);
            }

            // Right-Right Rotation
            Ship* rightChildNow = node->getRight();
            node->setRight(rightChildNow->getLeft());
            rightChildNow->setLeft(node);

            if (parent == nullptr)
                m_root = rightChildNow;

            else if (parent->getLeft() == node)
                parent->setLeft(rightChildNow);

            else
                parent->setRight(rightChildNow);
        }
        updateHeights(m_root);

    } while (imbalance(m_root) != nullptr);
}

// Performs Splay operations to bring the accessed node to the root
void Fleet::rotationSPLAY(Ship* shipNode) {
    if (shipNode != nullptr && m_root != shipNode) {
            while (m_root != shipNode) {
                Ship* greatGrandParent = nullptr;
                Ship* grandParent = nullptr;
                Ship* parent = nullptr;
                Ship* child = m_root;

                // Find the parent and grandparent of insertedShip
                while (child != shipNode) {
                    greatGrandParent = grandParent;
                    grandParent = parent;
                    parent = child;
                    child = (shipNode->getID() < child->getID()) ? child->getLeft() : child->getRight();
                }

                // Zig Case (Single Rotation)
                if (parent == m_root) {
                    if (shipNode == parent->getLeft()) {
                        // Right Rotation (Zig)
                        parent->setLeft(shipNode->getRight());
                        shipNode->setRight(parent);
                    }

                    else {
                        // Left Rotation (Zag)
                        parent->setRight(shipNode->getLeft());
                        shipNode->setLeft(parent);
                    }
                    m_root = shipNode;
                }
                // Zig-Zig or Zig-Zag or Zag-Zig or Zag-Zag Case
                else {
                    if (parent == grandParent->getLeft()) {
                        if (shipNode == parent->getLeft()) {
                            // Zig-Zig (Right-Right Rotation)
                            grandParent->setLeft(parent->getRight());
                            parent->setRight(grandParent);
                            parent->setLeft(shipNode->getRight());
                            shipNode->setRight(parent);
                        }
                        else {
                            // Zig-Zag (Right-Left Rotation)
                            parent->setRight(shipNode->getLeft());
                            grandParent->setLeft(shipNode->getRight());
                            shipNode->setLeft(parent);
                            shipNode->setRight(grandParent);
                        }
                    }
                    else {
                        if (shipNode == parent->getRight()) {
                            // Zag-Zag (Left-Left Rotation)
                            grandParent->setRight(parent->getLeft());
                            parent->setLeft(grandParent);
                            parent->setRight(shipNode->getLeft());
                            shipNode->setLeft(parent);
                        }
                        else {
                            // Zag-Zig (Left-Right Rotation)
                            parent->setLeft(shipNode->getRight());
                            grandParent->setRight(shipNode->getLeft());
                            shipNode->setRight(parent);
                            shipNode->setLeft(grandParent);
                        }
                    }

                    // Update greatGrandParent's child pointer
                    if (greatGrandParent != nullptr) {
                        if (grandParent == greatGrandParent->getLeft())
                            greatGrandParent->setLeft(shipNode);

                        else
                            greatGrandParent->setRight(shipNode);

                    }
                    else
                        // Update the root if grandParent was the root
                        m_root = shipNode;
                }
            }
            updateHeights(m_root);
    }
}

// Recursively copies a tree for assignment operator
Ship* Fleet::assignmentOperatorHelper(Ship* otherNode) {
    if (otherNode != nullptr) {
        int shipID = otherNode->getID();
        SHIPTYPE shipType = otherNode->getType();
        STATE shipState = otherNode->getState();

        Ship *newShip = new Ship(shipID, shipType, shipState);

        newShip->setLeft(assignmentOperatorHelper(otherNode->getLeft()));
        newShip->setRight(assignmentOperatorHelper(otherNode->getRight()));

        return newShip;
    }

    return nullptr;
}

void Fleet::dumpTree() const
{
    dump(m_root);
}

void Fleet::dump(Ship* aShip) const{
    if (aShip != nullptr){
        cout << "(";
        dump(aShip->m_left);//first visit the left child
        cout << aShip->m_id << ":" << aShip->m_height;//second visit the node itself
        dump(aShip->m_right);//third visit the right child
        cout << ")";
    }
}