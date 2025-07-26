# Spaceship Fleet

A class project done for CMSC 341 (Data Structures), focused on managing a fleet of spaceships using a flexible binary tree structure. Ships are organized by name and stored with details like type, build year, and strength — all built without using STL containers.

## What It Does

Supports inserting, removing, searching, and displaying ships using a tree that can operate in different modes:
- **BST** – standard binary search
- **AVL** – self-balancing
- **Splay** – brings frequently accessed nodes closer to the root

## File Breakdown

- `fleet.h` / `fleet.cpp`: Core logic for ship objects and tree operations
- `mytest.cpp`: Custom test file to validate features and edge cases
- `driver.cpp`: Sample driver that reads structured input
- `driver.txt`: Expected output when running the driver

## How to Use

- Build and run `mytest.cpp` to test your implementation
- Run `driver.cpp` to simulate input-driven behavior
- Compare the output with `driver.txt` to verify correctness

## Highlights

- Switchable tree modes: BST, AVL, Splay
- Manual memory management, no STL
- Proper handling of deep copy, assignment, and cleanup
- Covers edge cases like duplicates, empty trees, and rebalancing

## Notes

Ships are always sorted alphabetically by name. This project is all about understanding tree mechanics, memory management, and clean C++ logic — straight to the point, no extra fluff.
