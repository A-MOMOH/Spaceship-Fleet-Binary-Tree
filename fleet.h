// UMBC - CMSC 341 - Spring 2025 - Proj2
#ifndef FLEET_H
#define FLEET_H
#include <iostream>
using namespace std;
class Grader;//this class is for grading purposes, no need to do anything
class Tester;//this is your tester class, you add your test functions in this class
class Fleet;
enum STATE {ALIVE, LOST};   // possible states for a ship
enum SHIPTYPE {CARGO, TELESCOPE, COMMUNICATOR, FUELCARRIER, ROBOCARRIER};
enum TREETYPE {NONE, BST, AVL, SPLAY};
const int MINID = 10000;    // min ship ID
const int MAXID = 99999;    // max ship ID
#define DEFAULT_HEIGHT 0
#define DEFAULT_ID 0
#define DEFAULT_TYPE CARGO
#define DEFAULT_STATE ALIVE

class Ship{
    public:
    friend class Fleet;
    friend class Grader;
    friend class Tester;
    Ship(int id, SHIPTYPE type = DEFAULT_TYPE, STATE state = DEFAULT_STATE)
        :m_id(id),m_type(type), m_state(state) {
            m_left = nullptr;
            m_right = nullptr;
            m_height = DEFAULT_HEIGHT;
        }
    Ship(){
        m_id = DEFAULT_ID;
        m_type = DEFAULT_TYPE;
        m_state = DEFAULT_STATE;
        m_left = nullptr;
        m_right = nullptr;
        m_height = DEFAULT_HEIGHT;
    }
    int getID() const {return m_id;}
    STATE getState() const {return m_state;}
    string getStateStr() const {
        string text = "";
        switch (m_state)
        {
        case LOST:text = "LOST";break;
        case ALIVE:text = "ALIVE";break;
        default:text = "UNKNOWN";break;
        }
        return text;
    }
    SHIPTYPE getType() const {return m_type;}
    string getTypeStr() const {
        string text = "";
        switch (m_type)
        {
        case CARGO:text = "CARGO";break;
        case TELESCOPE:text = "TELESCOPE";break;
        case COMMUNICATOR:text = "COMMUNICATOR";break;
        case FUELCARRIER:text = "FUELCARRIER";break;
        case ROBOCARRIER:text = "ROBOCARRIER";break;
        default:text = "UNKNOWN";break;
        }
        return text
        ;
    }
    int getHeight() const {return m_height;}
    Ship* getLeft() const {return m_left;}
    Ship* getRight() const {return m_right;}
    void setID(const int id){m_id=id;}
    void setState(STATE state){m_state=state;}
    void setType(SHIPTYPE type){m_type=type;}
    void setHeight(int height){m_height=height;}
    void setLeft(Ship* left){m_left=left;}
    void setRight(Ship* right){m_right=right;}
    private:
    int m_id;
    SHIPTYPE m_type;
    STATE m_state;
    Ship* m_left;  //the pointer to the left child in the BST
    Ship* m_right; //the pointer to the right child in the BST
    int m_height;   //the height of this node in the BST
};
class Fleet{
    public:
    friend class Grader;
    friend class Tester;
    Fleet();
    Fleet(TREETYPE type);
    ~Fleet();
    const Fleet & operator=(const Fleet & rhs);
    void clear();
    TREETYPE getType() const;
    void setType(TREETYPE type);
    void insert(const Ship& ship);
    void remove(int id);
    void dumpTree() const;
    private:
    Ship* m_root;  // the root of the BST
    TREETYPE m_type;// the type of tree

    // ***************************************************
    // Any private helper functions must be declared here!
    Ship* imbalance(Ship* node);

    void updateHeights(Ship* node);

    void clearHelper(Ship* root);

    void rotationAVL();

    void rotationSPLAY(Ship* shipNode);

    Ship* assignmentOperatorHelper(Ship* otherNode);
    // ***************************************************

    void dump(Ship* aShip) const;//helper for recursive traversal
};
#endif