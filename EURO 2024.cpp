#include <bits/stdc++.h>
#include <fstream>
#include <sstream>
#include "gnuplot-iostream.h"

using namespace std;
struct Player {
    string name;
    string position;
    int age;
    string club;
    double height;
    string foot;
    int caps;
    int goals;
    double marketValue;
    string country;
};

vector<Player> readDataset(const string& filename) {
    vector<Player> players;
    ifstream file(filename);
    string line;

    // Skip the header
    getline(file, line);

    while (getline(file, line)) {
        stringstream ss(line);
        string name, position, age, club, height, foot, caps, goals, marketValue, country;

        getline(ss, name, ',');
        getline(ss, position, ',');
        getline(ss, age, ',');
        getline(ss, club, ',');
        getline(ss, height, ',');
        getline(ss, foot, ',');
        getline(ss, caps, ',');
        getline(ss, goals, ',');
        getline(ss, marketValue, ',');
        getline(ss, country, ',');

        players.push_back({
            name, position, stoi(age), club, stod(height), foot, stoi(caps), stoi(goals), stod(marketValue), country
            });
    }

    return players;
}
void generateReport(const vector<Player>& players) {
    ofstream report("euro2024_players.txt");
    if (!report.is_open()) {
        cerr << "Failed to open report file!" << endl;
        return;
    }

    map<string, int> clubCount;
    map<string, double> clubMarketValue;
    double totalMarketValue = 0;
    double avgAge = 0;
    double avgHeight = 0;

    for (const auto& player : players) {
        clubCount[player.club]++;
        clubMarketValue[player.club] += player.marketValue;
        totalMarketValue += player.marketValue;
        avgAge += player.age;
        avgHeight += player.height;
    }

    avgAge /= players.size();
    avgHeight /= players.size();

    cout << "UEFA EURO 2024 Player Report\n";
    cout << "----------------------------\n";
    cout << "Total Players: " << players.size() << "\n";
    cout << "Total Market Value: €" << totalMarketValue << "\n";
    cout << "Average Age: " << avgAge << " years\n";
    cout << "Average Height: " << avgHeight << " cm\n";
    cout << "\nPlayers per Club:\n";
    for (const auto& club : clubCount) {
        cout << club.first << ": " << club.second << " players\n";
    }
    //cout << "\nMarket Value per Club:\n";
    //for (const auto& club : clubMarketValue) {
    //    cout << club.first << ": €" << club.second << "\n";
    //}

    report.close();
    cout << "Report generated successfully!" << endl;
}

//Visualize Data
void visualizeMarketValueDistribution(const vector<Player>& players) {
    Gnuplot gp;
    vector<double> marketValues;

    for (const auto& player : players) {
        marketValues.push_back(player.marketValue);
    }

    gp << "set title 'Market Value Distribution of UEFA EURO 2024 Players'\n";
    gp << "set xlabel 'Market Value (€)'\n";
    gp << "set ylabel 'Number of Players'\n";
    gp << "plot '-' using 1:2 with boxes title 'Market Value Distribution'\n";

    vector<pair<double, int>> data;
    for (double mv : marketValues) {
        data.push_back(make_pair(mv, count(marketValues.begin(), marketValues.end(), mv)));
    }
    gp.send1d(data);
}

//Average Market Value per Club
map<string, double> averageMarketValuePerClub(const vector<Player>& players) {
    map<string, double> totalMarketValue, playerCount;
    for (const auto& player : players) {
        totalMarketValue[player.club] += player.marketValue;
        playerCount[player.club]++;
    }

    map<string, double> avgMarketValue;
    for (const auto& club : totalMarketValue) {
        avgMarketValue[club.first] = club.second / playerCount[club.first];
    }

    return avgMarketValue;
}

// Age Distribution by Position
map<string, vector<int>> ageDistributionByPosition(const vector<Player>& players) {
    map<string, vector<int>> ageDistribution;
    for (const auto& player : players) {
        ageDistribution[player.position].push_back(player.age);
    }
    return ageDistribution;
}

//Correlation Between Age and Market Value
double correlationAgeMarketValue(const vector<Player>& players) {
    double sumX = 0, sumY = 0, sumXY = 0, sumX2 = 0, sumY2 = 0;
    int n = players.size();

    for (const auto& player : players) {
        sumX += player.age;
        sumY += player.marketValue;
        sumXY += player.age * player.marketValue;
        sumX2 += player.age * player.age;
        sumY2 += player.marketValue * player.marketValue;
    }

    double correlation = (n * sumXY - sumX * sumY) /
        sqrt((n * sumX2 - sumX * sumX) * (n * sumY2 - sumY * sumY));

    return correlation;
}

// Market Value Distribution by Position
map<string, vector<double>> marketValueDistributionByPosition(const vector<Player>& players) {
    map<string, vector<double>> marketValueDistribution;
    for (const auto& player : players) {
        marketValueDistribution[player.position].push_back(player.marketValue);
    }
    return marketValueDistribution;
}

// Player with Highest Market Value in Each Position
map<string, Player> highestMarketValuePlayerInPosition(const vector<Player>& players) {
    map<string, Player> highestValuePlayers;
    for (const auto& player : players) {
        if (highestValuePlayers.find(player.position) == highestValuePlayers.end() ||
            player.marketValue > highestValuePlayers[player.position].marketValue) {
            highestValuePlayers[player.position] = player;
        }
    }
    return highestValuePlayers;
}

// Countries with Highest Average Market Value
map<string, double> countriesWithHighestAvgMarketValue(const vector<Player>& players) {
    map<string, double> totalMarketValue, playerCount;
    for (const auto& player : players) {
        totalMarketValue[player.country] += player.marketValue;
        playerCount[player.country]++;
    }

    map<string, double> avgMarketValue;
    for (const auto& country : totalMarketValue) {
        avgMarketValue[country.first] = country.second / playerCount[country.first];
    }

    return avgMarketValue;
}

// Club with Highest Number of Players
string clubWithMostPlayers(const vector<Player>& players) {
    map<string, int> clubCount;
    for (const auto& player : players) {
        clubCount[player.club]++;
    }

    string topClub;
    int maxPlayers = 0;
    for (const auto& club : clubCount) {
        if (club.second > maxPlayers) {
            topClub = club.first;
            maxPlayers = club.second;
        }

        return topClub;
    }
}

// Average Player Height by Club
map<string, double> averagePlayerHeightByClub(const vector<Player>& players) {
    map<string, double> totalHeight, playerCount;
    for (const auto& player : players) {
        totalHeight[player.club] += player.height;
        playerCount[player.club]++;
    }

    map<string, double> avgHeight;
    for (const auto& club : totalHeight) {
        avgHeight[club.first] = club.second / playerCount[club.first];
    }

    return avgHeight;
}

// Most Common Foot
string mostCommonFoot(const vector<Player>& players) {
    map<string, int> footCount;
    for (const auto& player : players) {
        footCount[player.foot]++;
    }

    string commonFoot;
    int maxCount = 0;
    for (const auto& foot : footCount) {
        if (foot.second > maxCount) {
            commonFoot = foot.first;
            maxCount = foot.second;
        }
    }
    return commonFoot;
}


//Top 5 Clubs by Total Market Value
vector<pair<string, double>> topClubsByMarketValue(const vector<Player>& players) {
    map<string, double> clubMarketValue;
    for (const auto& player : players) {
        clubMarketValue[player.club] += player.marketValue;
    }

    vector<pair<string, double>> sortedClubs(clubMarketValue.begin(), clubMarketValue.end());
    sort(sortedClubs.begin(), sortedClubs.end(), [](const pair<string, double>& a, const pair<string, double>& b) {
        return b.second < a.second;
        });

    return vector<pair<string, double>>(sortedClubs.begin(), sortedClubs.begin() + 5);
}


int main() {
    string filename = "euro2024_players.csv"; // Path to your CSV file
    vector<Player> players = readDataset(filename);
    generateReport(players);

	visualizeMarketValueDistribution(players);

	map<string, double> avgMarketValue = averageMarketValuePerClub(players);
	cout << "\nAverage Market Value per Club:\n";
	for (const auto& club : avgMarketValue) {
		cout << club.first << ": €" << club.second << "\n";
	}

	cout << "\nAverage Age: " << accumulate(players.begin(), players.end(), 0, [](int sum, const Player& player) {
		return sum + player.age; }) / players.size() << " years\n";

    cout << "\nAverage Height: " << accumulate(players.begin(), players.end(), 0.0, [](double sum, const Player& player) {
		return sum + player.height; }) / players.size() << " cm\n";



    cout << "\nAge Distribution by Position:\n";
    map<string, vector<int>> ageDist = ageDistributionByPosition(players);
        for (const auto& position : ageDist) {
            cout << position.first << ": ";
            for (int age : position.second) {
                cout << age << " ";
            }
            cout << "\n";
        }

	cout << "\nCorrelation Between Age and Market Value: " << correlationAgeMarketValue(players) << endl;
	
    cout << "\nMarket Value Distribution by Position:\n";
    map<string, vector<double>> mvDist = marketValueDistributionByPosition(players);
    for (const auto& position : mvDist) {
        cout << position.first << ": ";
        for (double mv : position.second) {
            cout << "€" << mv << " ";
        }
        cout << "\n";
    }

    cout << "\nPlayer with Highest Market Value in Each Position:\n";
    map<string, Player> highestValuePlayers = highestMarketValuePlayerInPosition(players);
    for (const auto& player : highestValuePlayers) {
        cout << player.first << ": " << player.second.name << " (€" << player.second.marketValue << ")\n";
    }

    cout << "\nCountries with Highest Average Market Value:\n";
    map<string, double> topCountries = countriesWithHighestAvgMarketValue(players);
    for (const auto& country : topCountries) {
        cout << country.first << ": €" << country.second << "\n";
    }

    cout << "\nClub with Most Players: " << clubWithMostPlayers(players) << endl;

	cout << "\nAverage Player Height by Club:\n";
	map<string, double> avgHeight = averagePlayerHeightByClub(players);
	for (const auto& club : avgHeight) {
		cout << club.first << ": " << club.second << " cm\n";
	}

	cout << "\nMost Common Foot: " << mostCommonFoot(players) << endl;

    cout << "\nTop 5 Clubs by Total Market Value:\n";
	vector<pair<string, double>> topClubs = topClubsByMarketValue(players);
    for (const auto& club : topClubs) {
		cout << club.first << ": €" << club.second << "\n";
    }


    return 0;
}
