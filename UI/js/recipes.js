$(document).ready(function () {
    getRecipes(function (data) {
        console.log(data);

        renderCards(data);

    });
});


function getRecipes(callback) {
    apiCallGet("recipe/list", null, callback);
}

function filterRecipes(){
    $("#recipe-cards").html("");
    let tagList = $("#tags").tagsinput('items');
    for(let recipe of Object.values(recipes)){
        let foundTag = true;
        let recipeString = JSON.stringify(recipe).toLowerCase();
        for(let tag of tagList){
            if(recipeString.indexOf(tag.toLowerCase()) < 0){
                foundTag = false;
                break;
            }
        }
        if(foundTag){
            renderRecipe(recipe);
        }
    }
    
}
function filterRecipesDropdown(selectedTag) {
    $("#recipe-cards").html(""); // Clear current recipes

    for (let recipe of Object.values(recipes)) {
        let recipeString = JSON.stringify(recipe).toLowerCase();

        // Check if the selected tag exists in the recipe
        if (recipeString.indexOf(selectedTag.toLowerCase()) >= 0) {
            renderRecipe(recipe);
        }
    }

    // If no recipes were found, show a message
    if ($("#recipe-cards").html() === "") {
        $("#recipe-cards").html("<p>No recipes found for the selected filter.</p>");
    }
}


function renderRecipe(recipe){
    let html = `
        <div class="col-md-4">
          <div class="card mb-4" style="#card">
            <img src="../${recipe.image}" class="card-img-top" alt="${recipe.name}">
            <div class="card-body">
              <h5 class="card-title">${recipe.name}</h5>
              <a href="#" 
              class="btn btn-primary" style="background-color: #62bb2e; color: white; border: #294f14;" 
              onclick='openRecipeModal(${JSON.stringify(recipe)})'>View
                Recipe</a>
            </div>
          </div>
        </div>
        `;
        $("#recipe-cards").append(html);

}
let recipes = null;
function renderCards(data) {
    recipes = data;
    $("#recipe-cards").html("");
    for(let recipe of Object.values(data)){
        renderRecipe(recipe);
    }
}




function openRecipeModal(recipe) {
    // Update modal content
    document.getElementById("recipeTitle").innerText = recipe.name;
    document.getElementById("recipeImage").src = "../" + recipe.image;

    // Populate ingredients
    const ingredientsList = document.getElementById("recipeIngredients");
    ingredientsList.innerHTML = ''; // Clear existing list
    
    recipe.ingredients.forEach(ingredient => {
        const li = document.createElement("li");
        li.innerText = ingredient;
        ingredientsList.appendChild(li);
    });

    // Add instructions
    document.getElementById("recipeInstructions").innerText = recipe.instructions;
    if(recipe.cooktime > 0){
        document.getElementById("recipeTime").innerText = (recipe.cooktime / 60) + " Minutes";
    }else{
        document.getElementById("recipeTime").innerText = "Unavailable";
    }
    
    // if(recipe.calories == 0 && recipe.fat == 0 && recipe.satfat == 0 && recipe.carbs == 0 && recipe.fiber == 0 && recipe.sugar == 0 && recipe.protien == 0){
    //     document.getElementById("recipeCalories").innerText = recipe.calories;
    //     document.getElementById("recipeFat").innerText = recipe.fat;
    //     document.getElementById("recipeSatFat").innerText = recipe.satfat;
    //     document.getElementById("recipeCarbs").innerText = recipe.carbs;
    //     document.getElementById("recipeFiber").innerText = recipe.fiber;
    //     document.getElementById("recipeSugar").innerText = recipe.sugar;
    //     document.getElementById("recipeProtien").innerText = recipe.protien;
    // } else{
    //     document.getElementById("recipeCalories").innerText = recipe.calories;
    //     document.getElementById("recipeFat").innerText = recipe.fat;
    //     document.getElementById("recipeSatFat").innerText = recipe.satfat;
    //     document.getElementById("recipeCarbs").innerText = recipe.carbs;
    //     document.getElementById("recipeFiber").innerText = recipe.fiber;
    //     document.getElementById("recipeSugar").innerText = recipe.sugar;
    //     document.getElementById("recipeProtien").innerText = recipe.protien;
    // }

    

    // Show modal
    $('#recipeModal').modal('show');
}


