#include "json.hpp"
#include <fstream>              // For file handling
#include <iostream>             // For input/output
#include <unordered_set>        // For storing inventory
#include <vector>               // For storing recommendations
#include <string>               // For handling strings



// Using the nlohmann namespace for convenience
using json = nlohmann::json;
using namespace std;

// Struct to represent a recipe
struct Recipe {
    string id;
    string name;
    vector<string> ingredients;
    vector<string> tags;
};

// Function to load recipes from a JSON file
vector<Recipe> loadRecipes(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Could not open the file!" << endl;
        exit(1); // Exit if file can't be opened
    }

    json recipeJson;
    file >> recipeJson; // Parse the JSON data into the recipeJson object

    vector<Recipe> recipes;

    // Loop through each recipe in the JSON data (looping over the keys like "id559")
    for (const auto& item : recipeJson.items()) {
        const auto& recipeData = item.value();
        
        Recipe recipe;
        recipe.id = recipeData["id"];
        recipe.name = recipeData["name"];
        recipe.ingredients = recipeData["ingredients"].get<vector<string>>(); // Convert ingredients to vector
        recipe.tags = recipeData["tags"].get<vector<string>>(); // Convert tags to vector

        recipes.push_back(recipe);
    }

    return recipes;
}

// Function to suggest recipes based on available ingredients
vector<string> suggestRecipes(const vector<Recipe>& recipes, const vector<string>& availableIngredients) {
    vector<string> suggestedRecipes;

    for (const auto& recipe : recipes) {
        bool canMakeRecipe = true;

        // Check if all ingredients for the recipe are available
        for (const auto& ingredient : recipe.ingredients) {
            if (find(availableIngredients.begin(), availableIngredients.end(), ingredient) == availableIngredients.end()) {
                canMakeRecipe = false;
                break; // If one ingredient is missing, stop checking this recipe
            }
        }

        // If all ingredients are available, add recipe to suggestions
        if (canMakeRecipe) {
            suggestedRecipes.push_back(recipe.name);
        }
    }

    return suggestedRecipes;
}

int main() {
    // Open the JSON file
    std::ifstream file("db-recipes.json");

    if (!file.is_open()) {
        std::cerr << "Error opening the file!" << std::endl;
        return 1;
    }

    // Parse the JSON data
    json recipes;
    file >> recipes;
    file.close();

    // For testing, just print out the first recipe
    // Assuming the structure is like: { "id559": { "name": "Recipe Name", ...} }
    if (!recipes.empty()) {
        // Let's pick a recipe from the JSON (for example, "id559")
        auto recipe = recipes["id33"];

        // Print out some details from the recipe
        std::cout << "Recipe Name: " << recipe["name"] << std::endl;

        std::cout << "Ingredients: " << std::endl;
        for (const auto& ingredient : recipe["ingredients"]) {
            std::cout << "- " << ingredient << std::endl;
        }
    } else {
        std::cerr << "No recipes found in the JSON file!" << std::endl;
    }

    return 0;
}