#include <iostream>
#include <bits/stdc++.h>

using namespace std;

class SocialNetwork
{
private:
    unordered_map<string, unordered_set<string>> graph;

    // Helper function to display a list of friends
    void displayFriends(const unordered_set<string> &friends)
    {
        for (auto &friendName : friends)
        {
            cout << friendName << " ,";
        }
        cout << "\n";
    }

    unordered_map<string, int> calculatePopularityScores()
    {
        unordered_map<string, int> popularityScores;
        for (auto &user : graph)
        {
            popularityScores[user.first] = user.second.size(); // Number of friends
        }
        return popularityScores;
    }

    unordered_map<string, double> calculatePageRank(int iterations = 100, double dampingFactor = 0.85) {
        unordered_map<string, double> rank;
        unordered_map<string, double> tempRank;
        int totalUsers = graph.size();

        for (auto& user : graph) {
            rank[user.first] = 1.0 / totalUsers;
        }

        for (int i = 0; i < iterations; ++i) {
            for (auto& user : graph) {
                tempRank[user.first] = (1.0 - dampingFactor) / totalUsers;
            }

            for (auto& user : graph) {
                string currentUser = user.first;
                double currentRank = rank[currentUser];
                int outDegree = user.second.size();

                if (outDegree > 0) {
                    double share = (currentRank * dampingFactor) / outDegree;
                    for (auto& friendName : user.second) {
                        tempRank[friendName] += share;
                    }
                } else {
                    for (auto& allUsers : graph) {
                        tempRank[allUsers.first] += (currentRank * dampingFactor) / totalUsers;
                    }
                }
            }
            rank = tempRank;
        }

        return rank;
    }

public:
    // Adding a user to the social network
    void addUser(const string &user)
    {
        if (graph.find(user) == graph.end())
        {
            graph[user] = {};
            cout << "User " << user << " added to the network.\n";
        }
        else
        {
            cout << "User " << user << " already exists.\n";
        }
    }

    // Removing a user from the social network
    void removeUser(const string &user)
    {
        if (graph.find(user) == graph.end())
        {
            cout << "User " << user << " does not exist.\n";
            return;
        }

        // Removing the user from all their friends' lists
        for (auto it = graph.begin(); it != graph.end(); ++it)
        {
            it->second.erase(user);
        }

        // Removing the user from the network
        graph.erase(user);
        cout << "User " << user << " removed from the network.\n";
    }

    // Add a friendship between two users
    void addFriendship(const string &user1, const string &user2)
    {
        if (graph.find(user1) == graph.end() or graph.find(user2) == graph.end())
        {
            cout << "Both users must exist in the network.\n";
            return;
        }

        graph[user1].insert(user2);
        graph[user2].insert(user1);
        cout << "Friendship added between " << user1 << " and " << user2 << ".\n";
    }

    // Remove a friendship between two users
    void removeFriendship(const string &user1, const string &user2)
    {
        if (graph.find(user1) == graph.end() or graph.find(user2) == graph.end())
        {
            cout << "Both users must exist in the network.\n";
            return;
        }

        graph[user1].erase(user2);
        graph[user2].erase(user1);
        cout << "Friendship removed between " << user1 << " and " << user2 << ".\n";
    }

    // Display a user's friends
    void displayUserFriends(const string &user)
    {
        if (graph.find(user) == graph.end())
        {
            cout << "User " << user << " does not exist.\n";
            return;
        }

        cout << "Friends of " << user << ": ";
        displayFriends(graph[user]);
    }

    // Suggest friends for a user based on mutual connections
    void suggestFriends(string &user)
    {
        if (graph.find(user) == graph.end())
        {
            cout << "User " << user << " does not exist.\n";
            return;
        }

        unordered_map<string, int> mutualCounts;
        for (auto &friendName : graph[user])
        {
            for (auto &mutualFriend : graph[friendName])
            {
                if (mutualFriend != user && graph[user].find(mutualFriend) == graph[user].end())
                {
                    mutualCounts[mutualFriend]++;
                }
            }
        }

        vector<pair<string, int>> suggestions(mutualCounts.begin(), mutualCounts.end());
        sort(suggestions.begin(), suggestions.end(), [](auto &a, auto &b)
             { return a.second > b.second; });

        cout << "Friend suggestions for " << user << ":\n";
        bool flag = 0;
        for (auto it : suggestions)
        {
            flag = 1;
            auto suggestedFriend = it.first;
            auto count = it.second;
            cout << suggestedFriend << " (" << count << " mutual friends)\n";
        }
        if (!flag)
            cout << "no suggestions\n";
    }

    // Suggesting friends based on entire connectivity network of users
    void suggestFriendsnetworking(string &user)
    {
        if (graph.find(user) == graph.end())
        {
            cout << "User " << user << " does not exist.\n";
            return;
        }

        unordered_set<string> vis, suggestions;
        queue<pair<string, int>> q;
        q.push({user, 0});
        vis.insert(user);

        while (!q.empty())
        {
            auto curr = q.front().first;
            int d = q.front().second;
            q.pop();

            if (d >= 2)
                suggestions.insert(curr);

            for (auto &nbr : graph[curr])
            {
                if (vis.find(nbr) == vis.end())
                {
                    vis.insert(nbr);
                    q.push({nbr, d + 1});
                }
            }
        }

        bool flag = 0;
        for (auto suggestedfriend : suggestions)
        {
            flag = 1;
            cout << suggestedfriend << "\n";
        }
        if (!flag)
            cout << "no suggestions" << "\n";
    }

    double jaccardscore(unordered_set<string> &user1friends, unordered_set<string> &user2friends)
    {
        set<string> unionset;
        set<string> intersectionset;

        for (auto friendname : user1friends)
        {
            if (user2friends.find(friendname) != user2friends.end())
                intersectionset.insert(friendname);
            unionset.insert(friendname);
        }

        for (auto friendname : user2friends)
        {
            unionset.insert(friendname);
        }

        return (double)intersectionset.size() / unionset.size();
    }

    // suggesting friends based on user similarity
    void suggestFriendsClustering(string user)
    {
        if (graph.find(user) == graph.end())
        {
            std::cout << "User " << user << " does not exist.\n";
            return;
        }

        vector<pair<string, double>> suggestions;

        for (auto &it : graph)
        {
            auto curruser = it.first;
            auto friends = it.second;

            if (curruser != user and graph[user].find(curruser) == graph[user].end())
            {
                double similarity = jaccardscore(graph[user], friends);
                suggestions.push_back({curruser, similarity});
            }
        }

        sort(suggestions.begin(), suggestions.end(), [](auto &a, auto &b)
             { return a.second > b.second; });

        cout << "Friend suggestions for " << user << ":\n";
        bool flag = 0;
        for (auto it : suggestions)
        {
            flag = 1;
            auto suggestedFriend = it.first;
            auto s = it.second;
            cout << suggestedFriend << " (similarity = " << s << ")\n";
        }
        if (!flag)
            cout << "no suggestions\n";
    }

    void exportGraph(const string &filename)
    {
        ofstream outFile(filename);
        if (!outFile.is_open())
        {
            cout << "Error opening file for export: " << filename << "\n";
            return;
        }
        outFile << "graph SocialNetwork {\n";

        for (auto &it : graph)
        {
            auto user = it.first;
            auto friends = it.second;
            for (auto &friendname : friends)
            {
                if (user < friendname) // Avoid duplicate edges
                    outFile << "  \"" << user << "\" -- \"" << friendname << "\";\n";
            }
        }

        outFile << "}\n";
        outFile.close();
        cout << "Graph exported to " << filename << ".\n";
    }

    // Display leaderboard of the most popular users
    void displayLeaderboard()
    {
        unordered_map<string, int> popularityScores = calculatePopularityScores();

        vector<pair<string, int>> leaderboard(popularityScores.begin(), popularityScores.end());
        sort(leaderboard.begin(), leaderboard.end(), [](auto &a, auto &b)
             { return a.second > b.second; });

        cout << "Leaderboard: Most Popular Users\n";
        int rank = 1;
        for (auto &entry : leaderboard)
        {
            cout << rank << ". " << entry.first << " - " << entry.second << " friends\n";
            rank++;
        }
    }

    // Display social influence scores using PageRank
    void displaySocialInfluenceScores() {
        unordered_map<string, double> pageRankScores = calculatePageRank();

        vector<pair<string, double>> scores(pageRankScores.begin(), pageRankScores.end());
        sort(scores.begin(), scores.end(), [](auto& a, auto& b) {
            return a.second > b.second; 
        });

        cout << "Social Influence Scores (PageRank):\n";
        int rank = 1;
        for (auto& entry : scores) {
            cout << rank << ". " << entry.first << " - " << entry.second << "\n";
            rank++;
        }
    }
};

int main()
{
    SocialNetwork network;

    cout << "Welcome to the Social Network Simulator!\n";

    while (true)
    {
        cout << "\nMenu:\n"
             << "1. Add user\n"
             << "2. Remove user\n"
             << "3. Add friendship\n"
             << "4. Remove friendship\n"
             << "5. Display user's friends\n"
             << "6. Suggest friends (Mutual Connections)\n"
             << "7. Suggest friends (user network)\n"
             << "8. Suggest friends (Clustering)\n"
             << "9. Export graph to file\n"
             << "10. View leaderboard\n"
             << "11. View social influence scores\n"
             << "12. Exit\n"
             << "Enter your choice: ";

        int choice;
        cin >> choice;

        if (choice == 12)
            break;

        string user1, user2, filename;
        switch (choice)
        {
        case 1:
            cout << "Enter user name: ";
            cin >> user1;
            network.addUser(user1);
            break;
        case 2:
            cout << "Enter user name: ";
            cin >> user1;
            network.removeUser(user1);
            break;
        case 3:
            cout << "Enter first user name: ";
            cin >> user1;
            cout << "Enter second user name: ";
            cin >> user2;
            network.addFriendship(user1, user2);
            break;
        case 4:
            cout << "Enter first user name: ";
            cin >> user1;
            cout << "Enter second user name: ";
            cin >> user2;
            network.removeFriendship(user1, user2);
            break;
        case 5:
            cout << "Enter user name: ";
            cin >> user1;
            network.displayUserFriends(user1);
            break;
        case 6:
            cout << "Enter user name: ";
            cin >> user1;
            network.suggestFriends(user1);
            break;
        case 7:
            cout << "Enter user name: ";
            cin >> user1;
            network.suggestFriendsnetworking(user1);
            break;
        case 8:
            cout << "Enter user name: ";
            cin >> user1;
            network.suggestFriendsClustering(user1);
            break;
        case 9:
            cout << "Enter file name to export the graph (e.g., network.dot): ";
            cin >> filename;
            network.exportGraph(filename);
            break;
        case 10:
            network.displayLeaderboard();
            break;
        case 11:
            network.displaySocialInfluenceScores();
            break;
        default:
            cout << "Invalid choice. Try again.\n";
        }
    }

    cout << "Thank you for using the Social Network Simulator!\n";
    return 0;
}
