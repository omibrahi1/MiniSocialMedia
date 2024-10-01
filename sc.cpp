#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
// #include <vector>

using namespace std;

template<typename T>
class DynamicList {
private:
    T* array;
    size_t capacity;
    size_t size;

    void resize(size_t newCapacity) {
        T* newArray = new T[newCapacity];
        for (size_t i = 0; i < size; ++i) {
            newArray[i] = array[i];
        }
        delete[] array;
        array = newArray;
        capacity = newCapacity;
    }

public:
    DynamicList() : array(nullptr), capacity(0), size(0) {}

    ~DynamicList() {
        delete[] array;
    }

    DynamicList(const DynamicList& other) : array(nullptr), capacity(0), size(0) {
        *this = other;
    }

    DynamicList& operator=(const DynamicList& other) {
        if (this != &other) {
            delete[] array;
            capacity = other.capacity;
            size = other.size;
            array = new T[capacity];
            for (size_t i = 0; i < size; ++i) {
                array[i] = other.array[i];
            }
        }
        return *this;
    }

    T& operator[](size_t index) {
        if (index >= size) {
            throw out_of_range("Index out of range");
        }
        return array[index];
    }

    const T& operator[](size_t index) const {
        if (index >= size) {
            throw out_of_range("Index out of range");
        }
        return array[index];
    }

    void push_back(const T& value) {
        if (size >= capacity) {
            size_t newCapacity = (capacity == 0) ? 1 : capacity * 2;
            resize(newCapacity);
        }
        array[size++] = value;
    }

    size_t getSize() const {
        return size;
    }
};

// Post class

class Post {
private:
    string text;
    string author;

public:
    Post(const string& txt, const string& auth) : text(txt), author(auth) {}

    const string& getText() const {
        return text;
    }

    const string& getAuthor() const {
        return author;
    }
};

// User class

class User {
private:
    string name;
    string username;
    string password;
    DynamicList<Post*> posts;
    DynamicList<User*> followedUsers;

public:
    User(const string& n, const string& uname, const string& pwd) : name(n), username(uname), password(pwd) {}

    const string& getName() const {
        return name;
    }

    const string& getUsername() const {
        return username;
    }

    const string& getPassword() const {
        return password;
    }

    void setName(const string& n) {
        name = n;
    }

    void setUsername(const string& uname) {
        username = uname;
    }

    void setPassword(const string& pwd) {
        password = pwd;
    }

    bool checkPassword(const string& pwd) const {
        return password == pwd;
    }

    void createPost(const string& text) {
        posts.push_back(new Post(text, username));
    }

    const DynamicList<Post*>& getPosts() const {
        return posts;
    }

    void sendFollowRequest(User* user) {
        // Check if the user is already being followed
        for (size_t i = 0; i < followedUsers.getSize(); ++i) {
            if (followedUsers[i] == user) {
                cout << "You are already following this user.\n";
                return;
            }
        }
        // Send follow request
        user->receiveFollowRequest(this);
        cout << "Follow request sent to " << user->getUsername() << ".\n";
    }

    void receiveFollowRequest(User* follower) {
        // Implement logic to receive and handle follow request
        // For simplicity, we'll automatically accept the request
        followedUsers.push_back(follower);
        cout << "Follow request from " << follower->getUsername() << " accepted.\n";
    }

    void viewFollowedUsers() const {
        cout << "Followed users:\n";
        for (size_t i = 0; i < followedUsers.getSize(); ++i) {
            cout << "- " << followedUsers[i]->getUsername() << endl;
        }
    }
};

// UserManager class

class UserManager {
private:
    DynamicList<User*> users;

public:
    ~UserManager() {
        for (size_t i = 0; i < users.getSize(); ++i) {
            delete users[i];
        }
    }

    User* findUserByUsername(const string& username) const {
        // Search for a user by username
        for (size_t i = 0; i < users.getSize(); ++i) {
            if (users[i]->getUsername() == username) {
                return users[i];
            }
        }
        return nullptr; // User not found
    }

    void acceptFollowRequest(User* currentUser, User* follower) {
        // Accept a follow request
        currentUser->receiveFollowRequest(follower);
    }

    void declineFollowRequest(User* currentUser, User* follower) {
        // Decline a follow request
        cout << "Follow request from " << follower->getUsername() << " declined.\n";
    }

    void registerUser(const string& name, const string& username, const string& password) {
        for (size_t i = 0; i < users.getSize(); ++i) {
            if (users[i]->getUsername() == username) {
                cout << "Username already exists. Please choose another username.\n";
                return;
            }
        }
        users.push_back(new User(name, username, password));
        cout << "User registered successfully.\n";
    }

    User* loginUser(const string& username, const string& password) {
        for (size_t i = 0; i < users.getSize(); ++i) {
            if (users[i]->getUsername() == username && users[i]->checkPassword(password)) {
                cout << "Login successful.\n";
                return users[i];
            }
        }
        cout << "Invalid username or password.\n";
        return nullptr;
    }

    void updateUserProfile(User* user, const string& newName, const string& newUsername, const string& newPassword) {
        // Check if the new username already exists
        for (size_t i = 0; i < users.getSize(); ++i) {
            if (users[i]->getUsername() == newUsername && users[i] != user) {
                cout << "Username already exists. Please choose another username.\n";
                return;
            }
        }
        // Update user profile
        user->setName(newName);
        user->setUsername(newUsername);
        user->setPassword(newPassword);
        cout << "User profile updated successfully.\n";
    }

    void createPost(User* user, const string& text) {
        user->createPost(text);
        cout << "Post created successfully.\n";
    }

    void viewPosts(User* user) const {
        cout << "Posts by " << user->getUsername() << ":\n";
        const DynamicList<Post*>& posts = user->getPosts();
        for (size_t i = 0; i < posts.getSize(); ++i) {
            cout << "Post " << i + 1 << ":\n";
            cout << "Author: " << posts[i]->getAuthor() << "\n";
            cout << "Text: " << posts[i]->getText() << "\n";
            cout << "\n";
        }
    }

    void sendFollowRequest(User* currentUser, const string& username) {
        User* userToFollow = findUserByUsername(username);
        if (userToFollow) {
            currentUser->sendFollowRequest(userToFollow);
        } else {
            cout << "User not found.\n";
        }
    }

    void viewFollowedUsers(User* currentUser) const {
        currentUser->viewFollowedUsers();
    }

    // Function to save registered users to a file
    void saveRegisteredUsers(const string& filename) {
        ofstream file(filename);
        if (file.is_open()) {
            for (size_t i = 0; i < users.getSize(); ++i) {
                User* user = users[i];
                file << user->getName() << "," << user->getUsername() << "," << user->getPassword() << endl;
            }
            file.close();
            cout << "Registered users saved successfully.\n";
        } else {
            cerr << "Unable to open file: " << filename << endl;
        }
    }

    // Function to load registered users from a file
    void loadRegisteredUsers(const string& filename) {
        ifstream file(filename);
        if (file.is_open()) {
            string line;
            while (getline(file, line)) {
                stringstream ss(line);
                string name, username, password;
                getline(ss, name, ',');
                getline(ss, username, ',');
                getline(ss, password);
                users.push_back(new User(name, username, password));
            }
            file.close();
            cout << "Registered users loaded successfully.\n";
        } else {
            cerr << "Unable to open file: " << filename << endl;
        }
    }
};

// Function to save logged-in user to a file
void saveLoggedInUser(const string& username, const string& filename) {
    ofstream file(filename);
    if (file.is_open()) {
        file << username;
        file.close();
        cout << "Logged-in user saved successfully.\n";
    } else {
        cerr << "Unable to open file: " << filename << endl;
    }
}

// Function to load logged-in user from a file
string loadLoggedInUser(const string& filename) {
    ifstream file(filename);
    string username;
    if (file.is_open()) {
        getline(file, username);
        file.close();
        cout << "Logged-in user loaded successfully.\n";
    } else {
        cerr << "Unable to open file: " << filename << endl;
    }
    return username;
}

int main() {
    UserManager userManager;

    // Load registered users
    userManager.loadRegisteredUsers("registered_users.txt");

    // Load logged-in user
    string loggedInUsername = loadLoggedInUser("logged_in_user.txt");
    User* loggedInUser = nullptr;
    if (!loggedInUsername.empty()) {
        loggedInUser = userManager.findUserByUsername(loggedInUsername);
    }

    int choice;
    do {
        cout << "\nMenu:\n";
        cout << "1. Register\n";
        cout << "2. Login\n";
        cout << "3. Update Profile\n";
        cout << "4. Create Post\n";
        cout << "5. View My Posts\n";
        cout << "6. Send Follow Request\n";
        cout << "7. View Followed Users\n";
        cout << "8. Save and Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                string name, username, password;
                cout << "\nRegistration\n";
                cout << "Enter name: ";
                cin >> name;
                cout << "Enter username: ";
                cin >> username;
                cout << "Enter password: ";
                cin >> password;
                userManager.registerUser(name, username, password);
                break;
            }
            case 2: {
                cout << "\nLogin\n";
                cout << "Enter username: ";
                cin >> loggedInUsername;
                cout << "Enter password: ";
                string password;
                cin >> password;
                loggedInUser = userManager.loginUser(loggedInUsername, password);
                if (loggedInUser) {
                    cout << "Logged in as: " << loggedInUser->getUsername() << endl;
                    saveLoggedInUser(loggedInUsername, "logged_in_user.txt");
                }
                break;
            }
            case 3: {
                if (loggedInUser) {
                    string newName, newUsername, newPassword;
                    cout << "\nUpdate Profile\n";
                    cout << "Enter new name: ";
                    cin >> newName;
                    cout << "Enter new username: ";
                    cin >> newUsername;
                    cout << "Enter new password: ";
                    cin >> newPassword;
                    userManager.updateUserProfile(loggedInUser, newName, newUsername, newPassword);
                } else {
                    cout << "Please log in first.\n";
                }
                break;
            }
            case 4: {
                if (loggedInUser) {
                    string postText;
                    cout << "\nCreate Post\n";
                    cout << "Enter post text: ";
                    cin.ignore(); // Ignore newline character from previous input
                    getline(cin, postText);
                    userManager.createPost(loggedInUser, postText);
                } else {
                    cout << "Please log in first.\n";
                }
                break;
            }
            case 5: {
                if (loggedInUser) {
                    userManager.viewPosts(loggedInUser);
                } else {
                    cout << "Please log in first.\n";
                }
                break;
            }
            case 6: {
                if (loggedInUser) {
                    string username;
                    cout << "\nSend Follow Request\n";
                    cout << "Enter username of the user you want to follow: ";
                    cin >> username;
                    userManager.sendFollowRequest(loggedInUser, username);
                } else {
                    cout << "Please log in first.\n";
                }
                break;
            }
            case 7: {
                if (loggedInUser) {
                    cout << "\nView Followed Users\n";
                    userManager.viewFollowedUsers(loggedInUser);
                } else {
                    cout << "Please log in first.\n";
                }
                break;
            }
            case 8: {
                cout << "Saving registered users...\n";
                userManager.saveRegisteredUsers("registered_users.txt");
                cout << "Exiting program.\n";
                break;
            }
            default: {
                cout << "Invalid choice. Please try again.\n";
                break;
            }
        }
    } while (choice != 8);

    return 0;
}
