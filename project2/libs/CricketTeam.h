#ifndef __CRICKET_TEAM__
#define __CRICKET_TEAM__

#include <string>

using CricketType = std::string; // This can change to other types such as
                                 // double and int, not just string

struct Cricketer {
  std::string m_firstName = "";
  std::string m_lastName  = "";
  CricketType m_type      = "";

  // comparison operaters
  bool operator==(const Cricketer &other) const {
    return (m_firstName == other.m_firstName) &&
           (m_lastName == other.m_lastName);
  }

  bool operator<(const Cricketer &other) const {
    if (m_lastName < other.m_lastName)
      return true;
    if (m_firstName < other.m_firstName)
      return true;
    return false;
  }
};

struct Node {
  Cricketer m_value;
  Node     *m_prev = nullptr;
  Node     *m_next = nullptr;
};

class CricketTeam {

public:
  CricketTeam(); // Create an empty CricketTeam list

  // Return true if the CricketTeam list is empty, otherwise false.
  bool noTeam() const;

  // Return the number of matches on the CricketTeam list.
  int cricketerCount() const;

  /// @brief add new cricketer in the list with the first name, last name and
  /// the type. If the full name (both the first and last name) is not equal to
  /// any full name currently in the list then add it and return true. Elements
  /// should be added according to their last name. Elements with the same last
  /// name should be added according to their first names. Otherwise, make no
  /// change to the list and return false (indicating that the name is already
  /// in the list).
  /// @param firstName
  /// @param lastName
  /// @param value
  /// @return true if new cricketer is added, false if the player with the same
  /// full name already exists.
  bool addCricketer(const std::string &firstName,
                    const std::string &lastName,
                    const CricketType &value);

  /// @brief change the cricket type of the cricketer If the full name is equal
  /// to a full name currently in the list, then make that full name no longer
  /// map to the value it currently maps to, but instead map to the value of the
  /// third parameter; return true in this case.
  /// @param firstName
  /// @param lastName
  /// @param value
  /// @return true if first and last name found in the list, otherwise, make no
  /// change to the list and return false.
  bool substituteCricketer(const std::string &firstName,
                           const std::string &lastName,
                           const CricketType &value);

  /// @brief Add or replace cricketer. If full name is equal to a name currently
  /// in the list, then make that full name no longer map to the value it
  /// currently maps to, but instead map to the value of the third parameter;
  /// return true in this case. If the full name is not equal to any full name
  /// currently in the list then add it and return true.
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
  /// equal to a full name currently in the list, set value to the value in the
  /// list that that full name maps to, and return true. Otherwise, make no
  /// change to the value parameter of this function and return false.
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
  Node root;
  int  m_count = 0;
};

#endif