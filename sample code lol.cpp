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

// Class for Community Posts
class Post {
public:
    std::string postId;
    std::string userId; // User who created the post
    std::string content;
    std::time_t timestamp;

    Post(const std::string& id, const std::string& userId, const std::string& content)
        : postId(id), userId(userId), content(content) {
        timestamp = std::time(nullptr);
    }
};

// GameMarketplace Class to manage overall system
class GameMarketplace 
{
private:
    std::vector<User*> users;
    std::vector<Game*> games;
    std::vector<Administrator*> administrators;
    std::vector<Post*> communityPosts;

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

    void populateWithDefaults() 
    {
        // Create 5 default admin users
        for (int i = 1; i <= 5; ++i) {
            std::string adminId = "admin" + std::to_string(i);
            administrators.push_back(new Administrator(adminId, adminId));
        }

        // Create 5 default customer users
        for (int i = 1; i <= 5; ++i) {
            std::string userId = "user" + std::to_string(i);
            users.push_back(new User(userId, userId, userId + "@example.com", "password", UserRole::CUSTOMER));
        }

        // Create 10 default games
        for (int i = 1; i <= 10; ++i) {
            std::string gameId = "game" + std::to_string(i);
            Game* newGame = new Game(gameId, "Game " + std::to_string(i), "Description " + std::to_string(i), 
                                      19.99 + i, "Genre " + std::to_string(i), 
                                      static_cast<GameRating>(i % 5), "Developer " + std::to_string(i));
            games.push_back(newGame);

            // Add reviews to the first 5 games
            if (i <= 5) {
                for (int j = 1; j <= i; ++j) {
                    newGame->addReview("Review " + std::to_string(j), j);
                }
            }
        }

        // Add 2 default posts to the community tab
        communityPosts.push_back(new Post("post1", "user1", "This game is awesome!"));
        communityPosts.push_back(new Post("post2", "user2", "Anyone want to play?"));
    }

    void runTextUI() 
    {
        int choice;
        User* currentUser = nullptr; // To keep track of logged-in user
        do {
            std::cout << "\nWelcome to the Game Store!\n";
            std::cout << "Navigation:\n";
            std::cout << "1. Community " << (choice == 1 ? "(You're here)" : "") << std::endl;
            std::cout << "2. Wishlist " << (choice == 2 ? "(You're here)" : "") << std::endl;
            std::cout << "3. Browse Games " << (choice == 3 ? "(You're here)" : "") << std::endl;
            std::cout << "4. Library " << (choice == 4 ? "(You're here)" : "") << std::endl;
            // ... (Other options)
            std::cout << "0. Exit\n";
            std::cout << "Enter your choice: ";
            std::cin >> choice;

            switch (choice) 
            {
                case 1: { // Community
                    std::cout << "\nCommunity Tab:\n";
                    for (const auto& post : communityPosts) {
                        std::cout << post->userId << ": " << post->content << std::endl;
                    }
                    // ... (Add options to create posts, comment, etc.)
                    break;
                }
                case 2: // Wishlist
                    std::cout << "\nWishlist:\n";
                    if (currentUser) {
                        // ... (Display currentUser's wishlist)
                    } else {
                        std::cout << "You need to log in to view your wishlist.\n";
                    }
                    break;
                case 3: // Browse Games
                    std::cout << "\nBrowse Games:\n";
                    for (const auto& game : games) {
                        std::cout << game->getTitle() << " - $" << game->getPrice() << std::endl;
                    }
                    // ... (Add options to view game details, add to cart, etc.)
                    break;
                case 4: // Library
                    std::cout << "\nLibrary:\n";
                    if (currentUser) {
                        // ... (Display currentUser's library)
                    } else {
                        std::cout << "You need to log in to view your library.\n";
                    }
                    break;
                // ... (Handle other cases)
                case 0:
                    std::cout << "Exiting...\n";
                    break;
                default:
                    std::cout << "Invalid choice!\n";
            }
        } while (choice != 0);
    }
};

int main() {
    // Example usage
    GameMarketplace marketplace;
    marketplace.populateWithDefaults();
    marketplace.runTextUI();

    std::cout << "Video Game Sales Software Initialized with Defaults!" << std::endl;

    return 0;

}