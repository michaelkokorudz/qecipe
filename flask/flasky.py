from flask import request
import flask
from flask_cors import CORS
import json

app = flask.Flask(__name__)
CORS(app)


@app.route('/')
def home():
    return "Hello, Flask!"

@app.route('/recipe/<recipeID>')
def getRecipe(recipeID):
    return recipeData[recipeID]

@app.route('/recipe/list')
def getRecipes():
    return recipeData


with open('flask/recipe_database.json', 'r') as file:
    recipeData = json.load(file)


if __name__ == '__main__':
    app.run(debug=True)