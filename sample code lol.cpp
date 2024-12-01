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
    E,      // Everyone
    E10,    // Everyone 10+
    T,      // Teen
    M,      // Mature
    AO      // Adults Only
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
    std::vector<std::pair<std::string, int>> reviews;

public:
    Game(const std::string& id, const std::string& title, 
         const std::string& description, double price, 
         const std::string& genre, GameRating rating,
         const std::string& developer)
        : gameId(id), title(title), description(description), 
          price(price), genre(genre), rating(rating),
          developerName(developer), 
          averageUserRating(0.0), totalReviews(0) {
        releaseDate = std::time(nullptr);
    }

    // Getters
    std::string getGameId() const { return gameId; }
    std::string getTitle() const { return title; }
    double getPrice() const { return price; }
    GameRating getRating() const { return rating; }
    std::string getGenre() const { return genre; }
    double getAverageRating() const { return averageUserRating; }

    // Method to add review
    void addReview(const std::string& reviewText, int starRating) {
        if (starRating < 1 || starRating > 5) {
            throw std::invalid_argument("Rating must be between 1 and 5");
        }
        reviews.push_back(std::make_pair(reviewText, starRating));
        
        // Recalculate average rating
        totalReviews++;
        double totalStars = 0;
        for (const auto& review : reviews) {
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
    std::vector<Game*> library;
    std::vector<Game*> wishlist;

public:
    User(const std::string& id, const std::string& username, 
         const std::string& email, const std::string& password,
         UserRole role)
        : userId(id), username(username), email(email), 
          password(password), role(role) {}

    bool login(const std::string& enteredPassword) {
        return password == enteredPassword;
    }

    void addToLibrary(Game* game) {
        library.push_back(game);
    }

    void addToWishlist(Game* game) {
        wishlist.push_back(game);
    }

    // Method to review a game
    void reviewGame(Game* game, const std::string& reviewText, int rating) {
        // Check if the game is in the user's library
        auto it = std::find(library.begin(), library.end(), game);
        if (it != library.end()) {
            game->addReview(reviewText, rating);
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
    std::unordered_map<std::string, Game*> gameCatalog;

public:
    Administrator(const std::string& id, const std::string& username)
        : adminId(id), username(username) {}

    void addGameToCatalog(Game* game) {
        gameCatalog[game->getGameId()] = game;
    }

    void removeGameFromCatalog(const std::string& gameId) {
        auto it = gameCatalog.find(gameId);
        if (it != gameCatalog.end()) {
            gameCatalog.erase(it);
        }
    }

    void setWeeklySale(const std::string& gameId, double discountPercentage) {
        auto it = gameCatalog.find(gameId);
        if (it != gameCatalog.end()) {
            double currentPrice = it->second->getPrice();
            double discountedPrice = currentPrice * (1 - discountPercentage / 100);
            it->second->updatePrice(discountedPrice);
        }
    }
};

// GameMarketplace Class to manage overall system
class GameMarketplace {
private:
    std::vector<User*> users;
    std::vector<Game*> games;
    std::vector<Administrator*> administrators;

public:
    // Methods to register users, add games, etc.
    User* registerUser(const std::string& username, const std::string& email, 
                       const std::string& password, UserRole role) {
        User* newUser = new User(std::to_string(users.size() + 1), 
                                 username, email, password, role);
        users.push_back(newUser);
        return newUser;
    }

    Game* createGame(const std::string& title, const std::string& description, 
                     double price, const std::string& genre, 
                     GameRating rating, const std::string& developer) {
        Game* newGame = new Game(std::to_string(games.size() + 1), 
                                 title, description, price, 
                                 genre, rating, developer);
        games.push_back(newGame);
        return newGame;
    }

    // Search functionality
    std::vector<Game*> searchGames(const std::string& query) {
        std::vector<Game*> results;
        for (auto* game : games) {
            if (game->getTitle().find(query) != std::string::npos ||
                game->getGenre().find(query) != std::string::npos) {
                results.push_back(game);
            }
        }
        return results;
    }

    // Destructor to clean up dynamically allocated memory
    ~GameMarketplace() {
        for (auto* user : users) delete user;
        for (auto* game : games) delete game;
        for (auto* admin : administrators) delete admin;
    }
};

int main() {
    // Example usage
    GameMarketplace marketplace;

    // Create a game
    Game* game1 = marketplace.createGame(
        "Epic Adventure", 
        "An incredible journey through mystical lands", 
        49.99, 
        "Action-Adventure", 
        GameRating::T, 
        "Indie Studios"
    );

    // Register a user
    User* user1 = marketplace.registerUser(
        "GameMaster", 
        "gamer@example.com", 
        "securePassword123", 
        UserRole::CUSTOMER
    );

    // Add game to user's library
    user1->addToLibrary(game1);

    // User reviews the game
    user1->reviewGame(game1, "Amazing game, really enjoyed it!", 5);

    std::cout << "Video Game Sales Software Initialization Complete!" << std::endl;

    return 0;
}