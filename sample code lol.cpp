#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <stdexcept>
#include <ctime>
#include <limits>
#include <random> // Required for random number generation

// Forward declarations
class Game;
class User;
class Administrator;

// Enum for user roles
enum class UserRole {
  CUSTOMER,
  DEVELOPER,
  MANAGER,
  ADMINISTRATOR
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
  std::string getDeveloperName() const {
    return developerName;
  }
  const std::vector < std::pair < std::string, int >> & getReviews() const {
    return reviews;
  }
  std::time_t getReleaseDate() const {
    return releaseDate;
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

    //Getters
    std::string getUsername() const {
        return username;
    }
    std::string getUserId() const {
        return userId;
    }
    UserRole getRole() const {
        return role;
    }
    std::vector < Game * > & getLibrary() {
        return library;
    }
    std::vector < Game * > & getWishlist() {
        return wishlist;
    }

    void addToLibrary(Game * game) {
        library.push_back(game);
    }

  void addToWishlist(Game * game) {
    wishlist.push_back(game);
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

  std::string getAdminUsername() const {
        return username;
    }

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
        double currentPrice = it->second->getPrice();
        double discountedPrice = currentPrice * (1 - discountPercentage / 100);
        it->second->updatePrice(discountedPrice); // Update the price in the gameCatalog
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
class GameMarketplace{
  private:
    std::vector < User * > users;
    std::vector < Game * > games;
    std::vector < Administrator * > administrators;
    std::vector < Post * > communityPosts;
    std::vector<Game*> gamesOnSale;

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
    std::vector<Game*> searchGames(const std::string& title = "", 
                                    double minPrice = 0.0, 
                                    double maxPrice = std::numeric_limits<double>::max(),
                                    const std::string& category = "",
                                    GameRating rating = GameRating::E,
                                    std::time_t minReleaseDate = 0, 
                                    std::time_t maxReleaseDate = std::numeric_limits<std::time_t>::max()) 
    {
        std::vector<Game*> results;

        for (auto* game : games) 
        {
            // Check title (case-insensitive partial match)
            bool titleMatch = title.empty() || 
                game->getTitle().find(title) != std::string::npos;

            // Check price range
            bool priceMatch = game->getPrice() >= minPrice && game->getPrice() <= maxPrice;

            // Check category (genre)
            bool categoryMatch = category.empty() || 
                game->getGenre().find(category) != std::string::npos;

            // Check rating (exact match)
            bool ratingMatch = (rating == GameRating::E) || game->getRating() == rating;

            // Check release date range
            bool releaseDateMatch = game->getReleaseDate() >= minReleaseDate && 
                                    game->getReleaseDate() <= maxReleaseDate;

            // If all selected criteria match, add to results
            if (titleMatch && priceMatch && categoryMatch && ratingMatch && releaseDateMatch) 
            {
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

    // Create 10 default games with ratings and reviews, randomly assigned to developers
        std::random_device rd; 
        std::mt19937 gen(rd()); 
        std::uniform_int_distribution<> devDistrib(1, 2); // Distribution for developer index (1 or 2)

        GameRating ratings[] = {GameRating::E, GameRating::E10, GameRating::T, GameRating::M, GameRating::AO};
        for (int i = 1; i <= 10; ++i) {
        std::string gameId = "game" + std::to_string(i);
        GameRating selectedRating = ratings[(i-1) % 5];

        // Randomly select a developer
        int devIndex = devDistrib(gen);
        std::string developerName = "developer" + std::to_string(devIndex); 

        Game* newGame = new Game(
            gameId, 
            "Game " + std::to_string(i), 
            "Description " + std::to_string(i),
            19.99 + i, 
            "Genre " + std::to_string(i),
            selectedRating, 
            developerName 
        );

        games.push_back(newGame);

        // Add reviews to some games
        if (i % 2 == 0) {
            for (int j = 1; j <= i / 2; ++j) {
                newGame->addReview("Review " + std::to_string(j), j % 5 + 1); // Ratings from 1 to 5
            }
        }

        if (i <= 3) {
            gamesOnSale.push_back(newGame);
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
    Administrator * currentAdmin = nullptr;
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
      std::cout << "3. Search Games\n";
      std::cout << "4. Library\n";
      std::cout << (loggedIn ? "5. Logout\n" : "5. Login\n");
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
          std::cout << "2. navbar\n";
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

          std::cout << "3. navbar\n";

          std::cout << "Enter your choice: ";

          std::cin >> input;

          if (input == "1") { // Games on Sale

            std::cout << "\nGames on Sale:\n";
                        for (int i = 0; i < gamesOnSale.size(); ++i) {
                            std::cout << i + 1 << ". " << gamesOnSale[i]->getTitle() << " - $" << gamesOnSale[i]->getPrice() << std::endl;
                        }

          } else if (input == "2") { // View all Games

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
                    //GAME DETAILS
                    std::cout << "\nGame Details:\n";
                    std::cout << "Title: " << selectedGame -> getTitle() << std::endl;
                    std::cout << "Genre: " << selectedGame -> getGenre() << std::endl;
                    std::cout << "Description: " << selectedGame -> getDescription() << std::endl;
                    std::cout << "Price: $" << selectedGame -> getPrice() << std::endl;

                    std::string ratingString;
                    switch(selectedGame->getRating()) {
                        case GameRating::E: ratingString = "Everyone"; break;
                        case GameRating::E10: ratingString = "Everyone 10+"; break;
                        case GameRating::T: ratingString = "Teen"; break;
                        case GameRating::M: ratingString = "Mature"; break;
                        case GameRating::AO: ratingString = "Adults Only"; break;
                        default: ratingString = "Unknown"; break;
                    }
                    std::cout << "Maturity Rating: " << ratingString << std::endl;

                    std::cout << "Release Date: " << selectedGame -> getReleaseDate() << std::endl;
                    //End of details

                  // ... (Display other details)

                  std::cout << "\nOptions:\n";

                  std::cout << "1. Buy Game\n";

                  std::cout << "2. Add to Wishlist\n";

                  std::cout << "3. Review Game\n";

                  std::cout << "4. See Reviews\n";

                  std::cout << "5. back\n";

                  std::cout << "Enter your choice: ";

                  std::cin >> input;

                  if (input == "1") { //buy game

                    if (loggedIn) 
                    {

                        // Check if the game is already in the library
                        bool gameInLibrary = false;
                        for (const auto& libraryGame : currentUser->getLibrary()) 
                        {
                            if (libraryGame->getTitle() == selectedGame->getTitle()) 
                            {
                                gameInLibrary = true;
                                break;
                            }
                        }

                        if (!gameInLibrary) 
                            {
                                // Simulate basic purchase logic
                                // In a real system, you'd integrate payment processing here
                                currentUser->addToLibrary(selectedGame);
                                std::cout << "Game '" << selectedGame->getTitle() << "' purchased and added to your library.\n";
                            } 
                        else 
                            {
                                    std::cout << "You already own this game in your library.\n";
                            }

                    } else {

                      std::cout << "You need to be logged in to buy a game.\n";

                    }

                  } else if (input == "2") { //add to wishlist
                    if (loggedIn) 
                    {

                        // Check if the game is already in the wishlist
                        bool gameInWishlist = false;
                        for (const auto& WlGame : currentUser->getWishlist()) 
                        {
                            if (WlGame->getTitle() == selectedGame->getTitle()) 
                            {
                                gameInWishlist = true;
                                break;
                            }
                        }

                        if (!gameInWishlist) 
                            {
                                // Simulate basic purchase logic
                                // In a real system, you'd integrate payment processing here
                                currentUser->addToWishlist(selectedGame);
                                std::cout << "Game '" << selectedGame->getTitle() << "' added to your wish list.\n";
                            } 
                        else 
                            {
                                    std::cout << "You already have this game in your wish list.\n";
                            }

                    } else {

                      std::cout << "You need to be logged to add a game to the wishlist.\n";

                    }

                  } else if (input == "3") { //review game
                    if (loggedIn) 
                    {
                        // Check if the game is in the user's library
                        bool gameInLibrary = false;
                        for (const auto& libraryGame : currentUser->getLibrary()) 
                        {
                            if (libraryGame->getTitle() == selectedGame->getTitle()) 
                            {
                                gameInLibrary = true;
                                break;
                            }
                        }

                        if (gameInLibrary) 
                        {
                            int rating;
                            std::string reviewText;

                            std::cout << "Enter your review (text): ";
                            std::cin.ignore();
                            std::getline(std::cin, reviewText);

                            while (true) 
                            {
                                std::cout << "Enter rating (1-5 stars): ";
                                std::cin >> rating;

                                // Validate rating
                                if (rating >= 1 && rating <= 5) 
                                {
                                    try 
                                    {
                                        currentUser->reviewGame(selectedGame, reviewText, rating);
                                        std::cout << "Review submitted successfully!\n";
                                        break;
                                    } catch (const std::exception& e) 
                                    {
                                        std::cout << "Error: " << e.what() << std::endl;
                                    }
                                } 
                                else 
                                {
                                    std::cout << "Invalid rating. Please enter a rating between 1 and 5.\n";
                                }
                            }
                        } 
                        else 
                        {
                            std::cout << "You can only review games in your library.\n";
                        }
                    } 
                    else 
                    {

                      std::cout << "You need to be logged in to review a game.\n";

                    }

                  } else if (input == "4") { //see reviews

                    std::cout << "\nReviews for " << selectedGame -> getTitle() << ":\n";

                    if (selectedGame -> getAverageRating() > 0) {

                      for (const auto & review: selectedGame -> getReviews()) {

                        std::cout << review.first << " - " << review.second << " stars\n";

                      }

                      std::cout << "Average rating: " << selectedGame -> getAverageRating() << " stars\n";

                    } else {

                      std::cout << "There are no reviews for this game yet.\n";

                    }

                  } else if (input == "5") { //back

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

          } else if (input == "3") { //navbar

            break; // Go back to navbar

          } else {

            std::cout << "Invalid choice!\n";

          }

        }

    } else if (input == "3") { // Search Games
    std::cout << "Advanced Search Options:\n";
    
    // Initialize search parameters with default values
    std::string titleQuery = "";
    double minPrice = 0.0;
    double maxPrice = std::numeric_limits<double>::max();
    std::string categoryQuery = "";
    GameRating rating = GameRating::E;
    std::time_t minReleaseDate = 0;
    std::time_t maxReleaseDate = std::numeric_limits<std::time_t>::max();

    // Title search
    std::cout << "Enter game title (or press Enter to skip): ";
    std::cin.ignore();
    std::getline(std::cin, titleQuery);

    // Price range
    std::cout << "Enter minimum price (0 to skip): ";
    std::cin >> minPrice;
    std::cout << "Enter maximum price (enter a large number to skip): ";
    std::cin >> maxPrice;

    // Category/Genre
    std::cout << "Enter game category/genre (or press Enter to skip): ";
    std::cin.ignore();
    std::getline(std::cin, categoryQuery);

    // Rating
    std::cout << "Select game rating:\n";
    std::cout << "1. E (Everyone)\n";
    std::cout << "2. E10 (Everyone 10+)\n";
    std::cout << "3. T (Teen)\n";
    std::cout << "4. M (Mature)\n";
    std::cout << "5. AO (Adults Only)\n";
    std::cout << "0. Skip rating filter\n";
    std::cout << "Enter your choice: ";
    int ratingChoice;
    std::cin >> ratingChoice;

    switch(ratingChoice) {
        case 1: rating = GameRating::E; break;
        case 2: rating = GameRating::E10; break;
        case 3: rating = GameRating::T; break;
        case 4: rating = GameRating::M; break;
        case 5: rating = GameRating::AO; break;
        default: rating = GameRating::E; break;
    }

    // Release date range
    std::cout << "Enter earliest release date (Unix timestamp, 0 to skip): ";
    std::cin >> minReleaseDate;
    std::cout << "Enter latest release date (Unix timestamp, a large number to skip): ";
    std::cin >> maxReleaseDate;

    // Perform search
    std::vector<Game*> searchResults = searchGames(titleQuery, minPrice, maxPrice, 
                                                   categoryQuery, rating, 
                                                   minReleaseDate, maxReleaseDate);

    if (!searchResults.empty()) {
        std::cout << "\nSearch results:\n";
        for (const auto& game : searchResults) {
            std::cout << "- " << game->getTitle() 
                      << " (Price: $" << game->getPrice() 
                      << ", Genre: " << game->getGenre() 
                      << ", Rating: " << static_cast<int>(game->getRating()) 
                      << ")\n";
        }
    } else {
        std::cout << "No games found matching your search criteria.\n";
    }
      
     } else if (input == "4") { // Library

        if (loggedIn) {

          while (true) {

            std::cout << "\nLibrary:\n";


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

            std::cout << "3. navbar\n";

            std::cout << "Enter your choice: ";

            std::cin >> input;

            if (input == "1") { //view game in library

                std::string gameTitle;
                std::cout << "Enter the title of the game to view: ";
                std::cin.ignore(); // Ignore the newline character in buffer
                std::getline(std::cin, gameTitle);

                Game* selectedGame = nullptr;
                // Find the game in the user's library
                for (const auto& libraryGame : currentUser->getLibrary()) 
                {
                    if (libraryGame->getTitle() == gameTitle) 
                    {
                        selectedGame = libraryGame;
                        break;
                    }
                }

                if (selectedGame) 
                {
                    while (true) 
                    {
                        //GAME DETAILS
                        std::cout << "\nGame Details:\n";
                        std::cout << "Title: " << selectedGame -> getTitle() << std::endl;
                        std::cout << "Genre: " << selectedGame -> getGenre() << std::endl;
                        std::cout << "Description: " << selectedGame -> getDescription() << std::endl;
                        std::cout << "Price: $" << selectedGame -> getPrice() << std::endl;

                        std::string ratingString;
                        switch(selectedGame->getRating()) {
                            case GameRating::E: ratingString = "Everyone"; break;
                            case GameRating::E10: ratingString = "Everyone 10+"; break;
                            case GameRating::T: ratingString = "Teen"; break;
                            case GameRating::M: ratingString = "Mature"; break;
                            case GameRating::AO: ratingString = "Adults Only"; break;
                            default: ratingString = "Unknown"; break;
                        }
                        std::cout << "Maturity Rating: " << ratingString << std::endl;

                        std::cout << "Release Date: " << selectedGame -> getReleaseDate() << std::endl;
                        //End of details

                        std::cout << "\nOptions:\n";
                        std::cout << "1. Review Game\n";
                        std::cout << "2. See Reviews\n";
                        std::cout << "3. Delete from Library\n";
                        std::cout << "4. back\n";
                        std::cout << "Enter your choice: ";

                        std::cin >> input;

                        if (input == "1") 
                        { // Review game
                            int rating;
                            std::string reviewText;

                            std::cout << "Enter your review (text): ";
                            std::cin.ignore();
                            std::getline(std::cin, reviewText);

                            while (true) 
                            {
                                std::cout << "Enter rating (1-5 stars): ";
                                std::cin >> rating;

                                // Validate rating
                                if (rating >= 1 && rating <= 5) 
                                {
                                    try 
                                    {
                                        currentUser->reviewGame(selectedGame, reviewText, rating);
                                        std::cout << "Review submitted successfully!\n";
                                        break;
                                    } catch (const std::exception& e) 
                                    {
                                        std::cout << "Error: " << e.what() << std::endl;
                                    }
                                } 
                                else 
                                {
                                    std::cout << "Invalid rating. Please enter a rating between 1 and 5.\n";
                                }
                            }
                        } 
                        else if (input == "2") 
                        { // See reviews
                            std::cout << "\nReviews for " << selectedGame->getTitle() << ":\n";
                            if (selectedGame->getAverageRating() > 0) 
                            {
                                for (const auto& review : selectedGame->getReviews()) 
                                {
                                    std::cout << review.first << " - " << review.second << " stars\n";
                                }
                                std::cout << "Average rating: " << selectedGame->getAverageRating() << " stars\n";
                            } 
                            else 
                            {
                                std::cout << "There are no reviews for this game yet.\n";
                            }
                        } 
                        else if (input == "3") 
                        { // Delete from Library
                            // Find and remove the game from the library
                            auto& library = currentUser->getLibrary();
                            auto it = std::find(library.begin(), library.end(), selectedGame);
                            if (it != library.end()) 
                            {
                                library.erase(it);
                                std::cout << "Game '" << selectedGame->getTitle() << "' removed from your library.\n";
                                break; // Exit the game details menu
                            }
                        } 
                        else if (input == "4") 
                        { // Back
                            break;
                        } 
                        else 
                        {
                            std::cout << "Invalid choice!\n";
                        }
                    }
                } 
                else 
                {
                    std::cout << "Game not found in your library!\n";
                }

            } else if (input == "2") { //wishlist

              while (true) {

                std::cout << "\nWishlist:\n";

                // --- Changed: Library and Wishlist are now attached to user data ---

                if (currentUser) 
                {
                  for (const auto & game: currentUser -> getWishlist()) {
                    std::cout << "- " << game -> getTitle() << std::endl;
                  }
                } else 
                {
                  std::cout << "You need to log in to view your wishlist.\n";
                }

                std::cout << "\nOptions:\n";
                std::cout << "1. Add to Wishlist\n";
                std::cout << "2. View Game \n"; 
                std::cout << "3. back\n";
                std::cout << "Enter your choice: ";
                std::cin >> input;

                if (input == "1") {//add to wishlist

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

                } else if (input == "2") { //View Game
                        std::string gameTitle;
                        std::cout << "Enter the title of the game to view: ";
                        std::cin.ignore(); // Ignore the newline character in buffer
                        std::getline(std::cin, gameTitle);

                        Game* selectedGame = nullptr;
                        // Find the game in the user's wishlist
                        for (const auto& wishlistGame : currentUser->getWishlist()) {
                            if (wishlistGame->getTitle() == gameTitle) {
                                selectedGame = wishlistGame;
                                break;
                            }
                        }

                        if (selectedGame) {
                            while (true) {
                                //GAME DETAILS
                                std::cout << "\nGame Details:\n";
                                std::cout << "Title: " << selectedGame -> getTitle() << std::endl;
                                std::cout << "Genre: " << selectedGame -> getGenre() << std::endl;
                                std::cout << "Description: " << selectedGame -> getDescription() << std::endl;
                                std::cout << "Price: $" << selectedGame -> getPrice() << std::endl;

                                std::string ratingString;
                                switch(selectedGame->getRating()) {
                                    case GameRating::E: ratingString = "Everyone"; break;
                                    case GameRating::E10: ratingString = "Everyone 10+"; break;
                                    case GameRating::T: ratingString = "Teen"; break;
                                    case GameRating::M: ratingString = "Mature"; break;
                                    case GameRating::AO: ratingString = "Adults Only"; break;
                                    default: ratingString = "Unknown"; break;
                                }
                                std::cout << "Maturity Rating: " << ratingString << std::endl;

                                std::cout << "Release Date: " << selectedGame -> getReleaseDate() << std::endl;
                                //End of details

                                std::cout << "\nOptions:\n";
                                std::cout << "1. Buy Game\n";
                                std::cout << "2. Remove from Wishlist\n";
                                std::cout << "3. back\n";
                                std::cout << "Enter your choice: ";

                                std::cin >> input;

                                if (input == "1") { // Buy Game
                                    bool gameInLibrary = false;
                                    for (const auto& libraryGame : currentUser->getLibrary()) {
                                        if (libraryGame->getTitle() == selectedGame->getTitle()) {
                                            gameInLibrary = true;
                                            break;
                                        }
                                    }

                                    if (!gameInLibrary) {
                                        currentUser->addToLibrary(selectedGame);
                                        std::cout << "Game '" << selectedGame->getTitle() << "' purchased and added to your library.\n";
                                        
                                        // Remove from wishlist after purchase
                                        auto& wishlist = currentUser->getWishlist();
                                        auto it = std::find(wishlist.begin(), wishlist.end(), selectedGame);
                                        if (it != wishlist.end()) {
                                            wishlist.erase(it);
                                        }
                                    } else {
                                        std::cout << "You already own this game in your library.\n";
                                    }
                                } else if (input == "2") { // Remove from Wishlist
                                    auto& wishlist = currentUser->getWishlist();
                                    auto it = std::find(wishlist.begin(), wishlist.end(), selectedGame);
                                    if (it != wishlist.end()) {
                                        wishlist.erase(it);
                                        std::cout << "Game '" << selectedGame->getTitle() << "' removed from your wishlist.\n";
                                        break; // Exit the game details menu
                                    }
                                } else if (input == "3") { // Back
                                    break;
                                } else {
                                    std::cout << "Invalid choice!\n";
                                }
                            }
                        } else {
                            std::cout << "Game not found in your wishlist!\n";
                        }
                
                } else if (input == "3") {

                  break; // Go back to Library

                } else {

                  std::cout << "Invalid choice!\n";

                }
              }


            } else if (input == "3") { //navbar

              break; // Go back to navbar

            } else { //inval choice

              std::cout << "Invalid choice!\n";

            }

          }

        } else {

          std::cout << "You need to log in to view your library.\n";

        }

      } else if (input == "5") { // Login/Logout

        if (loggedIn) {
          currentUser = nullptr;
          currentAdmin = nullptr;
          loggedIn = false;
          std::cout << "Logged out successfully.\n";
        } 

        else {
          std::cout << "\nLogin:\n";
          std::cout << "1. Customer\n";
          std::cout << "2. Developer\n";
          std::cout << "3. Manager\n";
          std::cout << "4. Administrator\n"; 
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
            role = UserRole::ADMINISTRATOR; // Use ADMINISTRATOR role
          } else {
            std::cout << "Invalid role!\n";
            continue;
          }

          // ... (display user data for testing - adjust indices if needed)
                    // Display the 1st default user data for testing
          std::cout << "\nFor testing, use:\n";
          if (role == UserRole::CUSTOMER) {
            std::cout << "Username: " << users[0]->getUsername() << std::endl; 
          } else if (role == UserRole::DEVELOPER) {
            std::cout << "Username: " << users[2]->getUsername() << std::endl; 
          } else if (role == UserRole::MANAGER) {
            std::cout << "Username: " << users[4]->getUsername() << std::endl; 
          } else if (role == UserRole::ADMINISTRATOR) {
            std::cout << "Username: " << administrators[0]->getAdminUsername() << std::endl;
          }
          std::cout << "Password: password\n\n"; 

          std::string username, password;
          std::cout << "Enter username: ";
          std::cin >> username;
          std::cout << "Enter password: ";
          std::cin >> password;

          if (role == UserRole::ADMINISTRATOR) {
            // Administrator login
            for (const auto & admin : administrators) {
              if (admin->getAdminUsername() == username && password == "password") { 
                loggedIn = true;
                std::cout << "Login successful!\n";
                runAdminUI(admin); // Call the admin UI function
              }
            }
          } else {
            // Customer, Developer, Manager login
            for (const auto & user : users) {
              if (user->getUsername() == username && user->getRole() == role && user->login(password)) {
                currentUser = user;
                loggedIn = true;
                std::cout << "Login successful!\n";

                // Redirect to the appropriate UI based on role
                if (role == UserRole::DEVELOPER) {
                  runDeveloperUI(currentUser);
                } else if (role == UserRole::MANAGER) {
                  runManagerUI(currentUser); 
                } 
                break;
              }
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
        std::cout << "\nDeveloper UI\n";
        std::cout << "1. Change Game Price\n";
        std::cout << "2. View Sales History\n";
        std::cout << "3. List My Games\n"; // Added option to list games
        std::cout << "4. Logout\n"; // Shifted logout option to 4
        std::cout << "Enter your choice: ";
        std::cin >> input;

      if (input == "1") {
        std::string gameTitle;
        double newPrice;
        std::cout << "Enter the title of the game to change price: ";
        std::cin.ignore();
        std::getline(std::cin, gameTitle);

        Game* selectedGame = nullptr;
        for (const auto& game : games) {
          if (game->getTitle() == gameTitle && game->getDeveloperName() == currentUser->getUsername()) {
            selectedGame = game;
            break;
          }
        }

        if (selectedGame) {
          std::cout << "Enter the new price: $";
          std::cin >> newPrice;
          selectedGame->updatePrice(newPrice);
          std::cout << "Price updated successfully!\n";
        } else {
          std::cout << "Game not found or you don't have permission to change its price.\n";
        }

      } else if (input == "2") {
        std::cout << "\nSales History:\n";
        // Make up default sales history values
        for (const auto& game : games) {
          if (game->getDeveloperName() == currentUser->getUsername()) {
            int salesCount = rand() % 100 + 1; // Generate random sales count (1-100)
            std::cout << game->getTitle() << ": " << salesCount << " sales\n";
          }
		 }

      } else if (input == "3") {
        std::cout << "\nYour Games:\n";
        for (const auto& game : games) {
          if (game->getDeveloperName() == currentUser->getUsername()) {
            std::cout << "- " << game->getTitle() << std::endl;
             }
        }
      }else if (input == "4") {
        std::cout << "Logging out...\n";
        break; 
      } else {
        std::cout << "Invalid choice!\n";
      }
    }
  }

  void runManagerUI(User * currentUser) 
  {
    std::string input;

    while (true) {
      std::cout << "\nManager UI\n";
      std::cout << "1. Set Weekly Sale\n";
      std::cout << "2. View Developer Sales History\n"; // Placeholder for dev sales
      std::cout << "3. Logout\n";
      std::cout << "Enter your choice: ";
      std::cin >> input;

    if (input == "1") {
        std::string gameTitle;
        double discountPercentage;
        std::cout << "Enter the name of the game to put on sale: ";
        std::cin.ignore();
        std::getline(std::cin, gameTitle);

        Game* selectedGame = nullptr;
        for (const auto& game : games) 
        {
            if (game->getTitle() == gameTitle) 
            {
                selectedGame = game;
                break;
            }
        }

        if (selectedGame) 
        {
            std::cout << "Enter the discount percentage: ";
            std::cin >> discountPercentage;
            for (const auto& admin : administrators) 
            {
                admin->setWeeklySale(selectedGame->getGameId(), discountPercentage); 
            }
            std::cout << "Weekly sale set successfully!\n";
        } 
        else 
        {
            std::cout << "Game not found.\n";
        }

      } else if (input == "2") 
      {
        std::cout << "\nDeveloper Sales History:\n";
        for (const auto& user : users) 
        {
          if (user->getRole() == UserRole::DEVELOPER) 
          {
            int totalSales = 0;
            for (const auto& game : games) 
            {
              if (game->getDeveloperName() == user->getUsername()) 
              {
                int salesCount = rand() % 100 + 1; // Generate random sales count
                totalSales += salesCount;
              }
            }
            std::cout << user->getUsername() << ": " << totalSales << " sales\n";
          }
        }

      } else if (input == "3") {
        std::cout << "Logging out...\n";
        break;
      } else {
        std::cout << "Invalid choice!\n";
      }
    }
  }

    void runAdminUI(Administrator * currentAdmin)
    {
        //leave blank no functional requirements
        std::string input;
        while (true) 
        {
        std::cout << "\nAdministrator UI\n";
        std::cout << "Nothing to see here!\n";
        std::cout << "1. Logout\n";
        std::cout << "Enter your choice: ";
        std::cin >> input;

        if (input == "1") 
        {
            std::cout << "Logging out...\n";
            break; // Exit the admin UI loop
        } else 
        {
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