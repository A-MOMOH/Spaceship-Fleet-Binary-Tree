#include "fleet.h"
#include <math.h>
#include <algorithm>
#include <random>
#include <vector>

enum RANDOM {UNIFORMINT, UNIFORMREAL, NORMAL, SHUFFLE};
class Random {
public:
    Random(){}
    Random(int min, int max, RANDOM type=UNIFORMINT, int mean=50, int stdev=20) : m_min(min), m_max(max), m_type(type)
    {
        if (type == NORMAL){
            //the case of NORMAL to generate integer numbers with normal distribution
            m_generator = std::mt19937(m_device());
            //the data set will have the mean of 50 (default) and standard deviation of 20 (default)
            //the mean and standard deviation can change by passing new values to constructor
            m_normdist = std::normal_distribution<>(mean,stdev);
        }
        else if (type == UNIFORMINT) {
            //the case of UNIFORMINT to generate integer numbers
            // Using a fixed seed value generates always the same sequence
            // of pseudorandom numbers, e.g. reproducing scientific experiments
            // here it helps us with testing since the same sequence repeats
            m_generator = std::mt19937(10);// 10 is the fixed seed value
            m_unidist = std::uniform_int_distribution<>(min,max);
        }
        else if (type == UNIFORMREAL) { //the case of UNIFORMREAL to generate real numbers
            m_generator = std::mt19937(10);// 10 is the fixed seed value
            m_uniReal = std::uniform_real_distribution<double>((double)min,(double)max);
        }
        else { //the case of SHUFFLE to generate every number only once
            m_generator = std::mt19937(m_device());
        }
    }
    void setSeed(int seedNum){
        // we have set a default value for seed in constructor
        // we can change the seed by calling this function after constructor call
        // this gives us more randomness
        m_generator = std::mt19937(seedNum);
    }
    void init(int min, int max){
        m_min = min;
        m_max = max;
        m_type = UNIFORMINT;
        m_generator = std::mt19937(10);// 10 is the fixed seed value
        m_unidist = std::uniform_int_distribution<>(min,max);
    }
    void getShuffle(vector<int> & array){
        // this function provides a list of all values between min and max
        // in a random order, this function guarantees the uniqueness
        // of every value in the list
        // the user program creates the vector param and passes here
        // here we populate the vector using m_min and m_max
        for (int i = m_min; i<=m_max; i++){
            array.push_back(i);
        }
        shuffle(array.begin(),array.end(),m_generator);
    }

    void getShuffle(int array[]){
        // this function provides a list of all values between min and max
        // in a random order, this function guarantees the uniqueness
        // of every value in the list
        // the param array must be of the size (m_max-m_min+1)
        // the user program creates the array and pass it here
        vector<int> temp;
        for (int i = m_min; i<=m_max; i++){
            temp.push_back(i);
        }
        std::shuffle(temp.begin(), temp.end(), m_generator);
        vector<int>::iterator it;
        int i = 0;
        for (it=temp.begin(); it != temp.end(); it++){
            array[i] = *it;
            i++;
        }
    }

    int getRandNum(){
        // this function returns integer numbers
        // the object must have been initialized to generate integers
        int result = 0;
        if(m_type == NORMAL){
            //returns a random number in a set with normal distribution
            //we limit random numbers by the min and max values
            result = m_min - 1;
            while(result < m_min || result > m_max)
                result = m_normdist(m_generator);
        }
        else if (m_type == UNIFORMINT){
            //this will generate a random number between min and max values
            result = m_unidist(m_generator);
        }
        return result;
    }

    double getRealRandNum(){
        // this function returns real numbers
        // the object must have been initialized to generate real numbers
        double result = m_uniReal(m_generator);
        // a trick to return numbers only with two deciaml points
        // for example if result is 15.0378, function returns 15.03
        // to round up we can use ceil function instead of floor
        result = std::floor(result*100.0)/100.0;
        return result;
    }

    string getRandString(int size){
        // the parameter size specifies the length of string we ask for
        // to use ASCII char the number range in constructor must be set to 97 - 122
        // and the Random type must be UNIFORMINT (it is default in constructor)
        string output = "";
        for (int i=0;i<size;i++){
            output = output + (char)getRandNum();
        }
        return output;
    }

    int getMin(){return m_min;}
    int getMax(){return m_max;}
    private:
    int m_min;
    int m_max;
    RANDOM m_type;
    std::random_device m_device;
    std::mt19937 m_generator;
    std::normal_distribution<> m_normdist;//normal distribution
    std::uniform_int_distribution<> m_unidist;//integer uniform distribution
    std::uniform_real_distribution<double> m_uniReal;//real uniform distribution

};

class Tester {
    public:
        // Tests whether AVL tree remains balanced after many insertions
        bool testAVLBalanceAfterInsertions();
        // Tests whether BST property is preserved after insertions
        bool testBSTPropertyAfterInsertions();
        // Tests whether splay tree operations correctly bring accessed nodes to the root
        bool testSplayTreeOperations();
        // Tests whether height values in a splay tree are updated correctly after insertions
        bool testHeightAccuracySplay();
        // Tests normal case of removing a node from a BST
        bool testBSTRemoveNormalCase();
        // Tests edge case: removing the only node in a BST
        bool testBSTRemoveEdgeCase();
        // Tests whether the AVL tree remains balanced after multiple removals
        bool testAVLBalanceAfterRemovals();
        // Tests whether BST property is maintained after multiple removals from BST and AVL
        bool testBSTPropertyAfterAVLandBSTRemovals();
        // Tests height accuracy in a BST after multiple removals
        bool testBSTHeightAccuracyAfterRemovals();
        // Tests normal case for the assignment operator (copying one tree to another)
        bool testAssignmentOperatorNormalCase();
        // Tests assignment operator for the error case (empty tree assignment)
        bool testAssignmentOperatorErrorCase();

    private:
        // Recursive helper function to verify BST property
        bool checkBSTProperty(Ship* root, int minID, int maxID);
        // Recursive helper function that verifies if the height is accurate at each node
        void checkHeights(Ship* node, bool& ifHeightsCorrect);
        // Recursive helper function that checks if two trees are structurally identical
        void checkIfIdentical(Ship* root1, Ship* root2, bool& identical);
        // Helper function that searches for a ship with a given ID in a fleet
        bool findShip(Ship* root, int shipID);
        // Detects for an imbalance in AVL tree by checking balance factor
        bool checkImbalance(Ship* root);
};

// Test whether the AVL tree remains balanced after multiple insertions
bool Tester::testAVLBalanceAfterInsertions() {
    Fleet fleet(AVL);
    Random idGen(MINID, MAXID, SHUFFLE);
    Random typeGen(0, 4);

    // Generate a shuffled list of unique IDs
    vector<int> uniqueIDs;
    idGen.getShuffle(uniqueIDs);

    // Insert the first 300 unique IDs into the fleet
    for (int i = 0; i < 300 && i < uniqueIDs.size(); i++) {
        int shipID = uniqueIDs[i];
        SHIPTYPE shipType = static_cast<SHIPTYPE>(typeGen.getRandNum());
        fleet.insert(Ship(shipID, shipType, ALIVE));
    }

    bool isBalanced = checkImbalance(fleet.m_root) == false;
    return isBalanced && checkBSTProperty(fleet.m_root, MINID, MAXID);
}

// Test whether the BST tree property is preserved after multiple insertions
bool Tester::testBSTPropertyAfterInsertions() {
    Fleet fleet(BST);
    Random idGen(MINID, MAXID, SHUFFLE);
    Random typeGen(0, 4);

    // Generate a shuffled list of unique IDs
    vector<int> uniqueIDs;
    idGen.getShuffle(uniqueIDs);

    // Insert the first 150 unique IDs into the fleet
    for (int i = 0; i < 150 && i < uniqueIDs.size(); i++) {
        int shipID = uniqueIDs[i];
        SHIPTYPE shipType = static_cast<SHIPTYPE>(typeGen.getRandNum());
        fleet.insert(Ship(shipID, shipType, ALIVE));

    }
    return checkBSTProperty(fleet.m_root, MINID, MAXID);
}

// Test whether the Splay tree performs splay operations correctly
bool Tester::testSplayTreeOperations() {
    Fleet fleet(SPLAY);
    Random idGen(MINID, MAXID, SHUFFLE);
    Random typeGen(0, 4);
    bool ifSplayed = true;

    // Generate a shuffled list of unique IDs
    vector<int> uniqueIDs;
    idGen.getShuffle(uniqueIDs);

    // Insert the first 150 unique IDs into the fleet
    for (int i = 0; i < 150 && i < uniqueIDs.size(); i++) {
        int shipID = uniqueIDs[i];
        SHIPTYPE shipType = static_cast<SHIPTYPE>(typeGen.getRandNum());
        fleet.insert(Ship(shipID, shipType, ALIVE));

        // Checks if the inserted node is splayed to the root
        if (fleet.m_root->getID() != shipID)
            ifSplayed = false;
    }

    return ifSplayed && checkBSTProperty(fleet.m_root, MINID, MAXID);
}

// Test whether the heights of nodes in a Splay tree are accurate
bool Tester::testHeightAccuracySplay() {
    Fleet fleet(SPLAY);
    Random idGen(MINID, MAXID, SHUFFLE);
    Random typeGen(0, 4);

    // Generate a shuffled list of unique IDs
    vector<int> uniqueIDs;
    idGen.getShuffle(uniqueIDs);

    // Insert the first 150 unique IDs into the fleet
    for (int i = 0; i < 150 && i < uniqueIDs.size(); i++) {
        int shipID = uniqueIDs[i];
        SHIPTYPE shipType = static_cast<SHIPTYPE>(typeGen.getRandNum());
        fleet.insert(Ship(shipID, shipType, ALIVE));
    }

    bool ifHeightsCorrect = true;
    checkHeights(fleet.m_root, ifHeightsCorrect);
    return ifHeightsCorrect;
}

// Test the removal of a node in a BST under normal conditions
bool Tester::testBSTRemoveNormalCase() {
    Fleet fleet(BST);
    Random idGen(MINID, MAXID, SHUFFLE);
    Random typeGen(0, 4);

    // Generate a shuffled list of unique IDs
    vector<int> uniqueIDs;
    idGen.getShuffle(uniqueIDs);

    // Insert the first 150 unique IDs into the fleet
    for (int i = 0; i < 150 && i < uniqueIDs.size(); i++) {
        int shipID = uniqueIDs[i];
        SHIPTYPE shipType = static_cast<SHIPTYPE>(typeGen.getRandNum());
        fleet.insert(Ship(shipID, shipType, ALIVE));
    }

    int removeShipID;
    bool shipIDExists = false;

    // Use the Random class to generate a random ID to remove
    Random removeIDGen(MINID, MAXID);
    do {
        // Generate a random ID within the range of MINID to MAXID
        removeShipID = removeIDGen.getRandNum();

        // Check if the ship ID exists in the fleet
        shipIDExists = findShip(fleet.m_root, removeShipID);

    } while (!shipIDExists); // Repeat until a valid ID is found


    fleet.remove(removeShipID);
    return !findShip(fleet.m_root, removeShipID) && checkBSTProperty(fleet.m_root, MINID, MAXID);
}

// Tests edge case where a BST has only one node and it’s removed
bool Tester::testBSTRemoveEdgeCase() {
    Fleet fleet(BST);
    fleet.insert(Ship(MINID, COMMUNICATOR, LOST));
    fleet.remove(MINID);
    return fleet.m_root == nullptr;
}

// Test whether the AVL tree remains balanced after multiple removals
bool Tester::testAVLBalanceAfterRemovals() {
    Fleet fleet(AVL);
    Random idGen(MINID, MAXID, SHUFFLE);
    Random typeGen(0, 4);

    // Generate a shuffled list of unique IDs
    vector<int> uniqueIDs;
    idGen.getShuffle(uniqueIDs);

    // Insert the first 300 unique IDs into the fleet
    for (int i = 0; i < 300 && i < uniqueIDs.size(); i++) {
        int shipID = uniqueIDs[i];
        SHIPTYPE shipType = static_cast<SHIPTYPE>(typeGen.getRandNum());
        fleet.insert(Ship(shipID, shipType, ALIVE));
    }

    // Use the Random class to generate a random ID to remove
    Random removeIDGen(MINID, MAXID);

    // Remove 150 random nodes from the fleet
    for (int i = 0; i < 150; i++) {
        int removeShipID;
        bool shipIDExists = false;

        do {
            // Generate a random ID within the range of MINID to MAXID
            removeShipID = removeIDGen.getRandNum();

            // Check if the ship ID exists in the fleet
            shipIDExists = findShip(fleet.m_root, removeShipID);

        } while (!shipIDExists); // Repeat until a valid ID is found

        fleet.remove(removeShipID);
    }

    bool isBalanced = checkImbalance(fleet.m_root) == false;
    return isBalanced && checkBSTProperty(fleet.m_root, MINID, MAXID);
}

// Test whether the BST property is preserved after removals in both AVL and BST trees
bool Tester::testBSTPropertyAfterAVLandBSTRemovals() {
    Fleet fleetBST(BST);
    Fleet fleetAVL(AVL);
    Random idGen(MINID, MAXID, SHUFFLE);
    Random typeGen(0, 4);

    // Generate a shuffled list of unique IDs
    vector<int> uniqueIDs;
    idGen.getShuffle(uniqueIDs);

    // Insert the first 300 unique IDs into the BST fleet and AVL fleet
    for (int i = 0; i < 300 && i < uniqueIDs.size(); i++) {
        int shipID = uniqueIDs[i];
        SHIPTYPE shipType = static_cast<SHIPTYPE>(typeGen.getRandNum());

        fleetBST.insert(Ship(shipID, shipType, ALIVE));
        fleetAVL.insert(Ship(shipID, shipType, ALIVE));
    }

    // Use the Random class to generate a random ID to remove
    Random removeIDGen(MINID, MAXID);

    // Remove 150 random nodes from both the BST fleet and AVL fleet
    for (int i = 0; i < 150; i++) {
        int removeShipID;
        bool shipIDExists = false;

        do {
            // Generate a random ID to remove within the range of inserted IDs
            removeShipID = removeIDGen.getRandNum();

            // Check if the ship ID exists in both the BST fleet and AVL fleet
            shipIDExists = findShip(fleetBST.m_root, removeShipID) && findShip(fleetAVL.m_root, removeShipID);
        } while (!shipIDExists); // Repeat until a valid ID is found

        fleetBST.remove(removeShipID);
        fleetAVL.remove(removeShipID);
    }

    bool isAVLBalanced = checkImbalance(fleetAVL.m_root) == false;

    return isAVLBalanced && checkBSTProperty(fleetAVL.m_root, MINID, MAXID) &&
        checkBSTProperty(fleetBST.m_root, MINID, MAXID);
}

// Test whether the heights of nodes in a BST are accurate after multiple removals
bool Tester::testBSTHeightAccuracyAfterRemovals() {
    Fleet fleet(BST);
    Random idGen(MINID, MAXID, SHUFFLE);
    Random typeGen(0, 4);

    // Generate a shuffled list of unique IDs
    vector<int> uniqueIDs;
    idGen.getShuffle(uniqueIDs);

    // Insert the first 150 unique IDs into the BST fleet and AVL fleet
    for (int i = 0; i < 150 && i < uniqueIDs.size(); i++) {
        int shipID = uniqueIDs[i];
        SHIPTYPE shipType = static_cast<SHIPTYPE>(typeGen.getRandNum());

        fleet.insert(Ship(shipID, shipType, ALIVE));
    }

    Random removeIDGen(MINID, MAXID);

    // Remove 50 random nodes from the fleet
    for (int i = 0; i < 50; i++) {
        int removeShipID;
        bool shipIDExists = false;

        do {
            // Generate a random ID to remove within the range of inserted IDs
            removeShipID = removeIDGen.getRandNum();

            // Check if the ship ID exists in the fleet
            shipIDExists = findShip(fleet.m_root, removeShipID);
        } while (!shipIDExists); // Repeat until a valid ID is found

        fleet.remove(removeShipID);
    }

    bool ifHeightsCorrect = true;
    checkHeights(fleet.m_root, ifHeightsCorrect);
    return ifHeightsCorrect;
}

// Test the assignment operator under normal conditions
bool Tester::testAssignmentOperatorNormalCase() {
    Fleet fleetOG(BST);
    Random idGen(MINID, MAXID, SHUFFLE);
    Random typeGen(0, 4);

    // Generate a shuffled list of unique IDs
    vector<int> uniqueIDs;
    idGen.getShuffle(uniqueIDs);

    // Insert the first 100 unique IDs into the BST fleet and AVL fleet
    for (int i = 0; i < 100 && i < uniqueIDs.size(); i++) {
        int shipID = uniqueIDs[i];
        SHIPTYPE shipType = static_cast<SHIPTYPE>(typeGen.getRandNum());

        fleetOG.insert(Ship(shipID, shipType, ALIVE));
    }

    Fleet fleetCopy;
    fleetCopy = fleetOG;

    bool identical = true;
    checkIfIdentical(fleetOG.m_root, fleetCopy.m_root, identical);

    return identical && checkBSTProperty(fleetCopy.m_root, MINID, MAXID);
}

// Test the assignment operator under error conditions (assigning an empty tree)
bool Tester::testAssignmentOperatorErrorCase() {
    Fleet fleetOG, fleetCopy;
    fleetCopy = fleetOG;

    return fleetCopy.m_root == nullptr;
}

// Helper function to check if the BST property is preserved
bool Tester::checkBSTProperty(Ship *root, int minID, int maxID) {
    if (root == nullptr)
        return true;

    if (root->getID() < minID || root->getID() > maxID)
        return false;

    return (checkBSTProperty(root->getLeft(), minID, root->getID() - 1) &&
            checkBSTProperty(root->getRight(), root->getID() + 1, maxID));
}

// Helper function to check if the heights of nodes are accurate
void Tester::checkHeights(Ship *node, bool& ifHeightsCorrect) {
    if (node != nullptr && ifHeightsCorrect) {
        // Traverse the left subtree
        checkHeights(node->getLeft(), ifHeightsCorrect);

        // Traverse the right subtree
        checkHeights(node->getRight(), ifHeightsCorrect);

        int leftHeight = (node->getLeft() != nullptr) ? node->getLeft()->getHeight() : -1;
        int rightHeight = (node->getRight() != nullptr) ? node->getRight()->getHeight() : -1;

        int expectedHeight = (leftHeight > rightHeight ? leftHeight : rightHeight) + 1;

        if (node->getHeight() != expectedHeight)
            ifHeightsCorrect = false;

    }
}

// Helper function to check if two trees are identical
void Tester::checkIfIdentical(Ship *root1, Ship *root2, bool& identical) {
    if (!identical)
        return;

    if (root1 == nullptr && root2 == nullptr)
        return;

    if (root1 == nullptr || root2 == nullptr || root1->getID() != root2->getID()) {
        identical = false;
        return;
    }

    checkIfIdentical(root1->getLeft(), root2->getLeft(), identical);
    checkIfIdentical(root1->getRight(), root2->getRight(), identical);
}

// Helper function that searches for a ship with a given ID in a fleet
bool Tester::findShip(Ship* root, int shipID) {
    if (shipID >= MINID && shipID <= MAXID) {
        Ship* temp = root;

        while (temp != nullptr) {
            if (temp->getID() == shipID)
                return true; // Ship found

            else if (temp->getID() < shipID)
                temp = temp->getRight(); // Move to the right subtree

            else
                temp = temp->getLeft(); // Move to the left subtree
        }
    }
    return false;
}

// Detects for an imbalance in AVL tree by checking balance factor
bool Tester::checkImbalance(Ship* root) {
    if (root == nullptr)
        return false;

    // Calculate the balance factor
    int balanceFactor = (root->getLeft() != nullptr ? root->getLeft()->getHeight() : -1) -
                        (root->getRight() != nullptr ? root->getRight()->getHeight() : -1);

    // If this node is unbalanced, return true
    if (abs(balanceFactor) > 1)
        return true;

    // Check left and right subtrees for imbalance
    if (checkImbalance(root->getLeft()))
        return true;

    if (checkImbalance(root->getRight()))
        return true;

    return false;
}


int main() {
    Tester tester;

    cout << "Testing AVL balance after insertions: " <<
            (tester.testAVLBalanceAfterInsertions() ? "Passed" : "Failed") << endl;

    cout << "Testing BST property after insertions: " <<
            (tester.testBSTPropertyAfterInsertions() ? "Passed" : "Failed") << endl;

    cout << "Testing Splay tree operations: " << (tester.testSplayTreeOperations() ? "Passed" : "Failed") << endl;

    cout << "Testing Splay tree heights accuracy: " << (tester.testHeightAccuracySplay() ? "Passed" : "Failed") << endl;

    cout << "Testing BST remove normal case: " << (tester.testBSTRemoveNormalCase() ? "Passed" : "Failed") << endl;

    cout << "Testing BST remove edge case: " << (tester.testBSTRemoveEdgeCase() ? "Passed" : "Failed") << endl;

    cout << "Testing AVL balance after removals: " <<
            (tester.testAVLBalanceAfterRemovals() ? "Passed" : "Failed") << endl;

    cout << "Testing BST property after AVL and BST removals: " <<
            (tester.testBSTPropertyAfterAVLandBSTRemovals() ? "Passed" : "Failed") << endl;

    cout << "Testing BST heights accuracy after removals: " <<
            (tester.testBSTHeightAccuracyAfterRemovals() ? "Passed" : "Failed") << endl;

    cout << "Testing assignment operator normal case: "
        << (tester.testAssignmentOperatorNormalCase() ? "Passed" : "Failed") << endl;

    cout << "Testing assignment operator error case: " <<
            (tester.testAssignmentOperatorErrorCase() ? "Passed" : "Failed") << endl;
    
    return 0;
}
