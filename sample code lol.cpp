#include <iostream>

#include <string>

#include <vector>

#include <unordered_map>

#include <algorithm>

#include <stdexcept>

#include <ctime>


// Forward declarations

class Game;

class User;

class Administrator;

// Enum for user roles

enum class UserRole {

  CUSTOMER,

  DEVELOPER,

  MANAGER

};

// Enum for game ratings

enum class GameRating {

  E, // Everyone

  E10, // Everyone 10+

  T, // Teen

  M, // Mature

  AO // Adults Only

};

// Game Class

class Game {

  private:

    std::string gameId;

  std::string title;

  std::string description;

  double price;

  std::string genre;

  GameRating rating;

  std::time_t releaseDate;

  std::string developerName;

  double averageUserRating;

  int totalReviews;

  std::vector < std::pair < std::string,
  int >> reviews;

  public:

    Game(const std::string & id,
      const std::string & title,

        const std::string & description, double price,

          const std::string & genre, GameRating rating,

            const std::string & developer)

    : gameId(id),
  title(title),
  description(description),

  price(price),
  genre(genre),
  rating(rating),

  developerName(developer),

  averageUserRating(0.0),
  totalReviews(0) {

    releaseDate = std::time(nullptr);

  }

  // Getters

  std::string getGameId() const {
    return gameId;
  }

  std::string getTitle() const {
    return title;
  }

  double getPrice() const {
    return price;
  }

  GameRating getRating() const {
    return rating;
  }

  std::string getGenre() const {
    return genre;
  }

  double getAverageRating() const {
    return averageUserRating;
  }

  std::string getDescription() const {
    return description;
  }

  const std::vector < std::pair < std::string, int >> & getReviews() const {
    return reviews;
  }

  // Method to add review

  void addReview(const std::string & reviewText, int starRating) {

    if (starRating < 1 || starRating > 5) {

      throw std::invalid_argument("Rating must be between 1 and 5");

    }

    reviews.push_back(std::make_pair(reviewText, starRating));

    // Recalculate average rating

    totalReviews++;

    double totalStars = 0;

    for (const auto & review: reviews) {

      totalStars += review.second;

    }

    averageUserRating = totalStars / totalReviews;

  }

  // Method to update price

  void updatePrice(double newPrice) {

    if (newPrice < 0) {

      throw std::invalid_argument("Price cannot be negative");

    }

    price = newPrice;

  }

};

// User Class

class User {

  private:

    std::string userId;

  std::string username;

  std::string email;

  std::string password;

  UserRole role;

  std::vector < Game * > library;

  std::vector < Game * > wishlist;

  public:

    User(const std::string & id,
      const std::string & username,

        const std::string & email,
          const std::string & password,

            UserRole role)

    : userId(id),
  username(username),
  email(email),

  password(password),
  role(role) {}

  bool login(const std::string & enteredPassword) {

    return password == enteredPassword;

  }

  std::string getUsername() const {
    return username;
  }

  std::string getUserId() const {
    return userId;
  }

  UserRole getRole() const {
    return role;
  }

  void addToLibrary(Game * game) {

    library.push_back(game);

  }

  void addToWishlist(Game * game) {

    wishlist.push_back(game);

  }

  // Getters for library and wishlist

  std::vector < Game * > & getLibrary() {
    return library;
  }

  std::vector < Game * > & getWishlist() {
    return wishlist;
  }

  // Method to review a game

  void reviewGame(Game * game,
    const std::string & reviewText, int rating) {

    // Check if the game is in the user's library

    auto it = std::find(library.begin(), library.end(), game);

    if (it != library.end()) {

      game -> addReview(reviewText, rating);

    } else {

      throw std::runtime_error("You can only review games in your library");

    }

  }

};

// Administrator Class

class Administrator {

  private:

    std::string adminId;

  std::string username;

  std::unordered_map < std::string,
  Game * > gameCatalog;

  public:

    Administrator(const std::string & id,
      const std::string & username)

    : adminId(id),
  username(username) {}

  void addGameToCatalog(Game * game) {

    gameCatalog[game -> getGameId()] = game;

  }

  void removeGameFromCatalog(const std::string & gameId) {

    auto it = gameCatalog.find(gameId);

    if (it != gameCatalog.end()) {

      gameCatalog.erase(it);

    }

  }

  void setWeeklySale(const std::string & gameId, double discountPercentage) {

    auto it = gameCatalog.find(gameId);

    if (it != gameCatalog.end()) {

      double currentPrice = it -> second -> getPrice();

      double discountedPrice = currentPrice * (1 - discountPercentage / 100);

      it -> second -> updatePrice(discountedPrice);

    }

  }

};

// Class for Community Posts

class Post {

  public:

    std::string postId;

  std::string userId; // User who created the post

  std::string content;

  std::time_t timestamp;

  Post(const std::string & id,
    const std::string & userId,
      const std::string & content)

  : postId(id),
  userId(userId),
  content(content) {

    timestamp = std::time(nullptr);

  }

};

// GameMarketplace Class to manage overall system

class GameMarketplace

{

  private:

    std::vector < User * > users;

  std::vector < Game * > games;

  std::vector < Administrator * > administrators;

  std::vector < Post * > communityPosts;

  public:

    // Methods to register users, add games, etc.

    User * registerUser(const std::string & username,
      const std::string & email,

        const std::string & password, UserRole role) {

      User * newUser = new User(std::to_string(users.size() + 1),

        username, email, password, role);

      users.push_back(newUser);

      return newUser;

    }

  Game * createGame(const std::string & title,
    const std::string & description,

      double price,
      const std::string & genre,

        GameRating rating,
        const std::string & developer) {

    Game * newGame = new Game(std::to_string(games.size() + 1),

      title, description, price,

      genre, rating, developer);

    games.push_back(newGame);

    return newGame;

  }

  // Search functionality

  std::vector < Game * > searchGames(const std::string & query) {

    std::vector < Game * > results;

    for (auto * game: games) {

      if (game -> getTitle().find(query) != std::string::npos ||

        game -> getGenre().find(query) != std::string::npos) {

        results.push_back(game);

      }

    }

    return results;

  }

  // Destructor to clean up dynamically allocated memory

  ~GameMarketplace() {

    for (auto * user: users) delete user;

    for (auto * game: games) delete game;

    for (auto * admin: administrators) delete admin;

  }

  void populateWithDefaults() {

    // Create 2 default customer users

    for (int i = 1; i <= 2; ++i) {

      std::string userId = "customer" + std::to_string(i);

      users.push_back(new User(userId, userId, userId + "@example.com", "password", UserRole::CUSTOMER));

    }

    // Create 2 default developer users

    for (int i = 1; i <= 2; ++i) {

      std::string userId = "developer" + std::to_string(i);

      users.push_back(new User(userId, userId, userId + "@example.com", "password", UserRole::DEVELOPER));

    }

    // Create 2 default manager users

    for (int i = 1; i <= 2; ++i) {

      std::string userId = "manager" + std::to_string(i);

      users.push_back(new User(userId, userId, userId + "@example.com", "password", UserRole::MANAGER));

    }

    //Added 2 default admin users

    for (int i = 1; i <= 2; ++i) {

      std::string adminId = "admin" + std::to_string(i);

      administrators.push_back(new Administrator(adminId, adminId));

    }

    // Create 10 default games with ratings and reviews

    for (int i = 1; i <= 10; ++i) {

      std::string gameId = "game" + std::to_string(i);

      Game * newGame = new Game(gameId, "Game " + std::to_string(i), "Description " + std::to_string(i),

        19.99 + i, "Genre " + std::to_string(i),

        static_cast < GameRating > (i % 5), "Developer " + std::to_string(i));

      games.push_back(newGame);

      // Add reviews to some games

      if (i % 2 == 0) {

        for (int j = 1; j <= i / 2; ++j) {

          newGame -> addReview("Review " + std::to_string(j), j % 5 + 1); // Ratings from 1 to 5

        }

      }

    }

    // Add 3 default posts to the community tab

    communityPosts.push_back(new Post("post1", "customer1", "This game is awesome!"));

    communityPosts.push_back(new Post("post2", "customer2", "Anyone want to play?"));

    communityPosts.push_back(new Post("post3", "user3", "sigma"));

  }

  void runTextUI() {

    std::string input;

    User * currentUser = nullptr;

    bool loggedIn = false;

    while (true) {

      std::cout << "\nWelcome to the Game Store!\n";

      if (loggedIn) {

        std::cout << "Logged in as: " << currentUser -> getUsername() << " (";

        switch (currentUser -> getRole()) {

        case UserRole::CUSTOMER:

          std::cout << "Customer";

          break;

        case UserRole::DEVELOPER:

          std::cout << "Developer";

          break;

        case UserRole::MANAGER:

          std::cout << "Manager";

          break;

        }

        std::cout << ")\n";

      }

      std::cout << "Navigation:\n";

      std::cout << "1. Community\n";

      std::cout << "2. Browse Games\n";

      std::cout << "3. Library\n";

      std::cout << (loggedIn ? "4. Logout\n" : "4. Login\n");

      std::cout << "0. Exit\n";

      std::cout << "Enter your choice: ";

      std::cin >> input;

      if (input == "1") { // Community

        while (true) {

          std::cout << "\nCommunity Tab:\n";

          for (const auto & post: communityPosts) {

            std::cout << post -> userId << ": " << post -> content << std::endl;

          }

          std::cout << "\nOptions:\n";

          std::cout << "1. Write a post\n";

          std::cout << "2. Search Games\n"; // --- Changed: Added Search Games option ---

          std::cout << "3. navbar\n";

          std::cout << "Enter your choice: ";

          std::cin >> input;

          if (input == "1") {

            if (loggedIn) {

              std::cout << "Post: ";

              std::cin.ignore(); // Ignore the newline in buffer

              std::string content;

              std::getline(std::cin, content);

              communityPosts.push_back(new Post("post" + std::to_string(communityPosts.size() + 1),

                currentUser -> getUsername(), content));

            } else {

              std::cout << "You need to be logged in to write a post.\n";

            }

          } else if (input == "2") {

            // --- Changed: Implemented Search Games in Community ---

            std::string searchQuery;

            std::cout << "Enter search query: ";

            std::cin.ignore();

            std::getline(std::cin, searchQuery);

            std::vector < Game * > searchResults = searchGames(searchQuery);

            if (!searchResults.empty()) {

              std::cout << "\nSearch results:\n";

              for (const auto & game: searchResults) {

                std::cout << "- " << game -> getTitle() << std::endl;

              }

            } else {

              std::cout << "No games found matching your query.\n";

            }

          } else if (input == "3") {

            break; // Go back to navbar

          } else {

            std::cout << "Invalid choice!\n";

          }

        }

      } else if (input == "2") { // Browse Games

        while (true) {

          std::cout << "\nBrowse Games:\n";

          // --- Changed: Added Games on Sale as the first option ---

          std::cout << "1. Games on Sale\n";

          std::cout << "2. View All Games\n";

          std::cout << "3. Search Games\n"; // --- Changed: Added Search Games option ---

          std::cout << "4. navbar\n";

          std::cout << "Enter your choice: ";

          std::cin >> input;

          if (input == "1") {

            // ... (Implement Games on Sale logic)

          } else if (input == "2") {

            for (int i = 0; i < games.size(); ++i) {

              std::cout << i + 1 << ". " << games[i] -> getTitle() << std::endl;

            }

            std::cout << "\nOptions:\n";

            std::cout << "1. View Game\n";

            std::cout << "2. back\n";

            std::cout << "Enter your choice: ";

            std::cin >> input;

            if (input == "1") {

              std::string gameTitle;

              std::cout << "Enter the title of the game to view: ";

              std::cin.ignore(); // Ignore the newline character in buffer

              std::getline(std::cin, gameTitle);

              Game * selectedGame = nullptr;

              for (const auto & game: games) {

                if (game -> getTitle() == gameTitle) {

                  selectedGame = game;

                  break;

                }

              }

              if (selectedGame) {

                while (true) {

                  std::cout << "\nGame Details:\n";

                  std::cout << "Title: " << selectedGame -> getTitle() << std::endl;

                  std::cout << "Description: " << selectedGame -> getDescription() << std::endl;

                  std::cout << "Price: $" << selectedGame -> getPrice() << std::endl;

                  // ... (Display other details)

                  std::cout << "\nOptions:\n";

                  std::cout << "1. Buy Game\n";

                  std::cout << "2. Review Game\n";

                  std::cout << "3. See Reviews\n";

                  // --- Changed: Add to Wishlist is now in Library ---

                  //std::cout << "4. Add to Wishlist\n";

                  std::cout << "4. back\n"; // Shifted back option to 4

                  std::cout << "Enter your choice: ";

                  std::cin >> input;

                  if (input == "1") {

                    if (loggedIn) {

                      // ... (Implement buy game logic)

                    } else {

                      std::cout << "You need to be logged in to buy a game.\n";

                    }

                  } else if (input == "2") {

                    if (loggedIn) {

                      // ... (Implement review game logic)

                    } else {

                      std::cout << "You need to be logged in to review a game.\n";

                    }

                  } else if (input == "3") {

                    std::cout << "\nReviews for " << selectedGame -> getTitle() << ":\n";

                    if (selectedGame -> getAverageRating() > 0) {

                      for (const auto & review: selectedGame -> getReviews()) {

                        std::cout << review.first << " - " << review.second << " stars\n";

                      }

                      std::cout << "Average rating: " << selectedGame -> getAverageRating() << " stars\n";

                    } else {

                      std::cout << "There are no reviews for this game yet.\n";

                    }

                  } else if (input == "4") {

                    break; // Go back to browse games

                  } else {

                    std::cout << "Invalid choice!\n";

                  }

                }

              } else {

                std::cout << "Game not found!\n";

              }

            } else if (input == "2") {

              break; // Go back to browse games menu

            } else {

              std::cout << "Invalid choice!\n";

            }

            // --- Changed: Implemented Search Games in Browse Games ---

          } else if (input == "3") {

            std::string searchQuery;

            std::cout << "Enter search query: ";

            std::cin.ignore();

            std::getline(std::cin, searchQuery);

            std::vector < Game * > searchResults = searchGames(searchQuery);

            if (!searchResults.empty()) {

              std::cout << "\nSearch results:\n";

              for (const auto & game: searchResults) {

                std::cout << "- " << game -> getTitle() << std::endl;

              }

            } else {

              std::cout << "No games found matching your query.\n";

            }

          } else if (input == "4") {

            break; // Go back to navbar

          } else {

            std::cout << "Invalid choice!\n";

          }

        }

      } else if (input == "3") { // Library

        if (loggedIn) {

          while (true) {

            std::cout << "\nLibrary:\n";

            // --- Changed: Library and Wishlist are now attached to user data ---

            if (currentUser) {

              for (const auto & game: currentUser -> getLibrary()) {

                std::cout << "- " << game -> getTitle() << std::endl;

              }

            } else {

              std::cout << "You need to log in to view your library.\n";

            }

            std::cout << "\nOptions:\n";

            std::cout << "1. View Game\n";

            std::cout << "2. Wishlist\n";

            std::cout << "3. Search Games\n"; // --- Changed: Added Search Games option ---

            std::cout << "4. navbar\n";

            std::cout << "Enter your choice: ";

            std::cin >> input;

            if (input == "1") {

              // ... (Implement view game logic - similar to browse games, but with delete option)

            } else if (input == "2") {

              while (true) {

                std::cout << "\nWishlist:\n";

                // --- Changed: Library and Wishlist are now attached to user data ---

                if (currentUser) {

                  for (const auto & game: currentUser -> getWishlist()) {

                    std::cout << "- " << game -> getTitle() << std::endl;

                  }

                } else {

                  std::cout << "You need to log in to view your wishlist.\n";

                }

                std::cout << "\nOptions:\n";

                std::cout << "1. Add to Wishlist\n"; // --- Changed: Implemented add to wishlist logic ---

                std::cout << "2. back\n";

                std::cout << "Enter your choice: ";

                std::cin >> input;

                if (input == "1") {

                  std::string gameTitle;

                  std::cout << "Enter the title of the game to add to wishlist: ";

                  std::cin.ignore(); // Ignore the newline character in buffer

                  std::getline(std::cin, gameTitle);

                  Game * selectedGame = nullptr;

                  for (const auto & game: games) {

                    if (game -> getTitle() == gameTitle) {

                      selectedGame = game;

                      break;

                    }

                  }

                  if (selectedGame) {

                    currentUser -> addToWishlist(selectedGame);

                    std::cout << selectedGame -> getTitle() << " added to wishlist.\n";

                  } else {

                    std::cout << "Game not found!\n";

                  }

                } else if (input == "2") {

                  break; // Go back to Library

                } else {

                  std::cout << "Invalid choice!\n";

                }

              }

              // --- Changed: Implemented Search Games in Library ---

            } else if (input == "3") {

              std::string searchQuery;

              std::cout << "Enter search query: ";

              std::cin.ignore();

              std::getline(std::cin, searchQuery);

              std::vector < Game * > searchResults = searchGames(searchQuery);

              if (!searchResults.empty()) {

                std::cout << "\nSearch results:\n";

                for (const auto & game: searchResults) {

                  std::cout << "- " << game -> getTitle() << std::endl;

                }

              } else {

                std::cout << "No games found matching your query.\n";

              }

            } else if (input == "4") {

              break; // Go back to navbar

            } else {

              std::cout << "Invalid choice!\n";

            }

          }

        } else {

          std::cout << "You need to log in to view your library.\n";

        }

      } else if (input == "4") { // Login/Logout

        if (loggedIn) {

          currentUser = nullptr;

          loggedIn = false;

          std::cout << "Logged out successfully.\n";

        } else {

          // --- Changed: Implemented login logic with UI switching ---

          std::cout << "\nLogin:\n";

          std::cout << "1. Customer\n";

          std::cout << "2. Developer\n";

          std::cout << "3. Manager\n";

          std::cout << "4. Administrator\n"; // --- Changed: Added Admin login option ---

          std::cout << "Enter your role: ";

          std::cin >> input;

          UserRole role;

          if (input == "1") {

            role = UserRole::CUSTOMER;

          } else if (input == "2") {

            role = UserRole::DEVELOPER;

          } else if (input == "3") {

            role = UserRole::MANAGER;

          } else if (input == "4") {

            role = UserRole::MANAGER; // --- Changed: Treat Admin as Manager for now ---

          } else {

            std::cout << "Invalid role!\n";

            continue;

          }

          // --- Changed: Display one user data for testing ---

          std::cout << "\nFor testing, use:\n";

          if (role == UserRole::CUSTOMER) {

            std::cout << "Username: " << users[0] -> getUsername() << std::endl; // Display the first customer's username

          } else if (role == UserRole::DEVELOPER) {

            std::cout << "Username: " << users[2] -> getUsername() << std::endl; // Display the first developer's username

          } else if (role == UserRole::MANAGER) {

            std::cout << "Username: " << users[4] -> getUsername() << std::endl; // Display the first manager's username

          }

          std::cout << "Password: password\n";

          std::string username, password;

          std::cout << "Enter username: ";

          std::cin >> username;

          std::cout << "Enter password: ";

          std::cin >> password;

          for (const auto & user: users) {

            if (user -> getUsername() == username && user -> login(password)) {

              currentUser = user;

              loggedIn = true;

              std::cout << "Login successful!\n";

              // --- Changed: Switch to Admin UI if admin user ---


              break;

            }

          }

          if (!loggedIn) {

            std::cout << "Invalid username or password.\n";

          }

        }

      } else if (input == "0") { // Exit

        std::cout << "Exiting...\n";

        break;

      } else {

        std::cout << "Invalid choice!\n";

      }

    }

  }

  void runDeveloperUI(User * currentUser)

  {

    std::string input;

    while (true) {

      std::cout << "\nDeveloper Menu for " << currentUser -> getUsername() << ":\n";

      std::cout << "1. Community\n";

      std::cout << "2. Browse Games\n";

      std::cout << "3. Manage Games\n";

      std::cout << "4. Logout\n";

      std::cout << "0. Exit\n";

      std::cout << "Enter your choice: ";

      std::cin >> input;

      // Similar structure to customer UI with additional developer-specific options

      if (input == "4") {

        return; // Logout and return to main menu

      } else if (input == "0") {

        exit(0);

      } else {

        std::cout << "Invalid choice!\n";

      }

    }

  }

  void runManagerUI(User * currentUser)

  {

    std::string input;

    while (true) {

      std::cout << "\nManager Menu for " << currentUser -> getUsername() << ":\n";

      std::cout << "1. Community\n";

      std::cout << "2. Browse Games\n";

      std::cout << "3. Sales Reports\n";

      std::cout << "4. User Management\n";

      std::cout << "5. Logout\n";

      std::cout << "0. Exit\n";

      std::cout << "Enter your choice: ";

      std::cin >> input;

      // Similar structure to other UIs with manager-specific options

      if (input == "5") {

        return; // Logout and return to main menu

      } else if (input == "0") {

        exit(0);

      } else {

        std::cout << "Invalid choice!\n";

      }

    }

  }



};

int main() {

  // Example usage

  GameMarketplace marketplace;

  marketplace.populateWithDefaults();

  marketplace.runTextUI();

  std::cout << "Video Game Sales Software Demo Complete!! :)" << std::endl;

  return 0;

}