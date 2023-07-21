#ifndef __CRICKET_TEAM__
#define __CRICKET_TEAM__

#include <string>

using CricketType = std::string; // This can change to other types such as
                                 // double and int, not just string

class CricketTeam {
private:
  // Definition of node value
  struct Cricketer {
    std::string firstName   = "";
    std::string lastName    = "";
    CricketType cricketType = "";

    // comparison operaters

    /// @brief Check the Cricketer equals to another by first name and last name
    /// @param fn first name
    /// @param ln last name
    /// @return true if both first name and last name matches, othersize false
    bool equalsTo(const std::string &fn, const std::string &ln) const;

    /// @brief Check the Cricketer is less than another by last name and first
    /// name
    /// @param fn first name
    /// @param ln last name
    /// @return true if the last name < ln or first name < fn && last name ==
    /// ln, otherwise false
    bool lessThan(const std::string &fn, const std::string &ln) const;
  };

  struct Node {
    Cricketer value = Cricketer();
    Node     *prev  = nullptr;
    Node     *next  = nullptr;
  };

  // Private data members
  Node *start = new Node();
  Node *rear  = new Node();
  int  count = 0;

public:
  // Constructors and distructor
  CricketTeam(); // Create an empty CricketTeam list
  CricketTeam(const CricketTeam &other);
  virtual ~CricketTeam();

  CricketTeam &operator=(const CricketTeam &other);

  // Return true if the CricketTeam list is empty, otherwise false.
  bool noTeam() const { return count == 0; };

  // Return the number of matches on the CricketTeam list.
  int cricketerCount() const { return count; };

  /// @brief add new cricketer in the list with the first name, last name and
  /// the type. If the full name (both the first and last name) is not equal
  /// to any full name currently in the list then add it and return true.
  /// Elements should be added according to their last name. Elements with the
  /// same last name should be added according to their first names.
  /// Otherwise, make no change to the list and return false (indicating that
  /// the name is already in the list).
  /// @param firstName
  /// @param lastName
  /// @param value
  /// @return true if new cricketer is added, false if the player with the
  /// same full name already exists.
  bool addCricketer(const std::string &firstName,
                    const std::string &lastName,
                    const CricketType &value);

  /// @brief change the cricket type of the cricketer If the full name is
  /// equal to a full name currently in the list, then make that full name no
  /// longer map to the value it currently maps to, but instead map to the
  /// value of the third parameter; return true in this case.
  /// @param firstName
  /// @param lastName
  /// @param value
  /// @return true if first and last name found in the list, otherwise, make
  /// no change to the list and return false.
  bool substituteCricketer(const std::string &firstName,
                           const std::string &lastName,
                           const CricketType &value);

  /// @brief Add or replace cricketer. If full name is equal to a name
  /// currently in the list, then make that full name no longer map to the
  /// value it currently maps to, but instead map to the value of the third
  /// parameter; return true in this case. If the full name is not equal to
  /// any full name currently in the list then add it and return true.
  /// @param firstName
  /// @param lastName
  /// @param value
  /// @return true
  bool addOrSubstitute(const std::string &firstName,
                       const std::string &lastName,
                       const CricketType &value);

  /// @brief Remove a cricketer by full name
  /// @param firstName
  /// @param lastName
  /// @return true if the cricketer is removed, false if not found.
  bool releaseCricketer(const std::string &firstName,
                        const std::string &lastName);

  /// @brief Check a cricketer is in the list by full name
  /// @param firstName
  /// @param lastName
  /// @return true if the full name is equal to a full name currently in the
  /// list, otherwise false.
  bool rosteredOnCricketTeam(const std::string &firstName,
                             const std::string &lastName) const;

  /// @brief Search cricketer by name and get the type. If the full name is
  /// equal to a full name currently in the list, set value to the value in
  /// the list that that full name maps to, and return true. Otherwise, make
  /// no change to the value parameter of this function and return false.
  /// @param firstName
  /// @param lastName
  /// @param value
  /// @return true if found, otherwise false
  bool searchForCricketer(const std::string &firstName,
                          const std::string &lastName,
                          CricketType       &value) const;

  /// @brief Get cricketer information based on the position. If 0 <= i <
  /// size(), copy into firstName, lastName and value parameters the
  /// corresponding information of the element at position i in the list and
  /// return true. Otherwise, leave the parameters unchanged and return false.
  /// @param i
  /// @param firstName
  /// @param lastName
  /// @param value
  /// @return true if found, false if the index is out of range
  bool checkTeamForCricketer(int          i,
                             std::string &firstName,
                             std::string &lastName,
                             CricketType &value) const;
  // If 0 <= i < size(), copy into firstName, lastName and value
  // parameters the corresponding information of the element at
  // position i in the list and return true. Otherwise, leave the
  // parameters unchanged and return false. (See below for details
  // about this function.)

  /// @brief xchange the contents of this list with the other one.
  /// @param other
  void tradeCricketTeams(CricketTeam &other);

private:
  // auxiliary functions

  /// @brief Add a new Node in front of an existing node
  /// @param pos the pointer to the current node
  /// @param firstName
  /// @param lastName
  /// @param value cricket type
  void insertNode(Node              *pos,
                  const std::string &firstName,
                  const std::string &lastName,
                  const CricketType &value);

  void removeNode(Node* pos);
  
  /// @brief get the pointer to a node by first name and last name
  /// @param firstName
  /// @param lastName
  /// @return pointer to the node if found, otherwise nullptr
  Node *getNode(const std::string &firstName,
                const std::string &lastName) const {
    for (auto p = start->next; p != nullptr; p = p->next) {
      if (p->value.equalsTo(firstName, lastName)) {
        return p;
      }
    }
    return nullptr;
  }

  /// @brief get the smallest node that no less than given full name
  /// @param firstName
  /// @param lastName
  /// @return pointer to the node which is the first node no less than the given
  /// full name, if all the nodes in the list are less than the full name,
  /// return the rear node
  Node *lowerBound(const std::string &firstName,
                   const std::string &lastName) const {
    for (auto p = start->next; p != rear; p = p->next) {
      if (!p->value.lessThan(firstName, lastName)) {
        return p;
      }
    }

    return rear;
  }
};

/// @brief Combine 2 teams into one.When this function returns, odJoined must
/// consist of pairs determined by these rules:
///  - If a full name appears in exactly one of odOne and odTwo, then odJoined
///  must contain an element consisting of that full name and its corresponding
///  value.
///  - If a full name appears in both odOne and odTwo, with the same
///  corresponding valuein both, then odJoined must contain an element with that
///  full name and value.
///  - When this function returns, odJoined must contain no elements other than
///  those required by these rules. (You must not assume odJoined is empty when
///  it is passed in to this function; it might not be.)
///  - If there exists a full name that appears in both odOne and odTwo, but
///  with different corresponding values, thenthis function returns false; if
///  there is no full name like this, the function returns true. Even if the
///  function returns false, result must be constituted as defined by the above
///  rules.
/// @param odOne
/// @param odTwo
/// @param odJoined combined result of odOne and odTwo, previous nodes stored in odJoined will be deleted
/// @return
bool mergeCricketers(const CricketTeam &odOne,
                     const CricketTeam &odTwo,
                     CricketTeam       &odJoined);

/// @brief search cricketers and store the result in a list
/// @param fsearch first name to match, wildcard supported
/// @param lsearch last name to match, wildcard supported
/// @param odOne the list to search
/// @param odResult the nodes satisfied the search criteria
void checkCricketers(const std::string &fsearch,
                     const std::string &lsearch,
                     const CricketTeam &odOne,
                     CricketTeam       &odResult);
#endif
