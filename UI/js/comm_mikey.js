// comm_mikey.js

// Function to handle API calls
function apiCall(endpoint, data, method = "POST", callback) {
    $.ajax({
      url: endpoint,
      method: method,
      contentType: "application/x-www-form-urlencoded",
      data: data,
      success: function (response) {
        callback(null, response);
      },
      error: function (xhr, status, error) {
        callback({ status: xhr.status, statusText: xhr.statusText, error: error });
      },
    });
  }
  
  // Save the logged-in username to localStorage
  function saveUsername(username) {
    localStorage.setItem("username", username);
  }
  
  // Retrieve the logged-in username from localStorage
  function getUsername() {
    return localStorage.getItem("username");
  }
  
  // Login function
  function loginUser(username, password) {
    const data = { username, password };
  
    apiCall("http://localhost:8000/login_user", data, "POST", function (err, response) {
      if (err) {
        alert("Login failed: " + (err.error || err.statusText));
      } else {
        alert("Login successful!");
        saveUsername(username); // Save the username after successful login
        window.location.href = "dashboard.html"; // Redirect to dashboard
      }
    });
  }
  function signUpUser(username, password) {
    const data = { username };

    console.log("Checking if user exists with:", data); // Debugging log

    // Step 1: Check if the user exists
    apiCall("http://localhost:8000/check_user_exists", data, "POST", function (err, response) {
        if (err) {
            // If the error is a 404 (user not found), proceed to add the user
            if (err.status === 404) {
                console.log("User not found, proceeding to add user...");
                addUser(username, password); // Call the add user function
                window.location.href = "login.html"; // Redirect to login
            } else {
                console.error("Error checking user:", err);
                alert("Error checking user: " + (err.error || err.statusText));
            }
            return;
        }

        // If the user exists, handle appropriately
        if (response.trim() === "User exists") {
            alert("Username already exists. Please Login or use a different username.");
            window.location.href = "signup.html"; // Redirect to the login page
        } else {
            // Handle unexpected responses
            console.warn("Unexpected response:", response);
            alert("Unexpected response during user check: " + response);
        }
    });
}

// Add User Function
function addUser(username, password) {
    const signupData = { username, password };
    console.log("Adding user with:", signupData); // Debugging log

    apiCall("http://localhost:8000/add_user", signupData, "POST", function (err, response) {
        if (err) {
            console.error("Error adding user:", err);
            alert("Error during signup: " + (err.error || err.statusText));
            return;
        }

        if (response.trim() === "User added successfully") {
            alert("Signup successful! Please login.");
            window.location.href = "login.html"; // Redirect to login
        } else {
            // Handle unexpected responses
            console.warn("Unexpected response during signup:", response);
            alert("Unexpected response during signup: " + response);
        }
    });
}

let fridgeItems = []; // Array to store structured fridge items

function fetchFridgeItems() {
  const username = getUsername();
  if (!username) {
    alert("You must be logged in.");
    return;
  }

  apiCall("http://localhost:8000/get_fridge_items", { username }, "POST", function (err, response) {
    if (err) {
      alert("Failed to fetch fridge items: " + (err.error || err.statusText));
    } else {
      fridgeItems = parseFridgeItems(response); // Parse response
      populateFridgeTable(fridgeItems); // Update table
    }
  });
}

function parseFridgeItems(response) {
  // Split response into individual lines
  return response
    .trim()
    .split("\n")
    .map((line) => {
      // Extract fields using regular expressions
      const nameMatch = line.match(/Name: (.*?),/);
      const quantityMatch = line.match(/Quantity: (.*?),/);
      const unitMatch = line.match(/Unit: (.*?),/);
      const expirationMatch = line.match(/Expiration: (.*)$/);

      // Return structured object
      return {
        name: nameMatch ? nameMatch[1].trim() : "Unknown",
        quantity: quantityMatch ? parseInt(quantityMatch[1], 10) : 0,
        unit: unitMatch ? unitMatch[1].trim() : "N/A",
        expirationDate: expirationMatch ? expirationMatch[1].trim() : "N/A",
      };
    });
}

function populateFridgeTable(items) {
  const tableContainer = $("#inventoryTableContainer");
  const tableBody = $("#inventoryTableBody");
  tableBody.empty(); // Clear existing table content

  if (items.length === 0) {
    tableContainer.hide(); // Hide the table container
    return;
  }

  tableContainer.show(); // Ensure the table is visible

  items.forEach(({ name, quantity, unit, expirationDate }) => {
    // Skip items with invalid or missing fields
    if (!name || name === "Unknown" || !quantity || quantity === "N/A" || !unit || !expirationDate) {
      return;
    }

    const row = `
      <tr>
        <td>${name}</td>
        <td>${quantity}</td>
        <td>${unit}</td>
        <td>Fridge</td>
        <td>${expirationDate}</td>
        <td class="text-right">
          <button 
            class="btn btn-sm btn-danger" 
            data-name="${name}"
            data-quantity="${quantity}"
            data-unit="${unit}"
            data-expiration="${expirationDate}"
            onclick="openRemoveModal(this)">
            Remove
          </button>
        </td>
      </tr>
    `;
    tableBody.append(row);
  });
}

// Open the Remove Item Modal and populate fields
function openRemoveModal(button) {
  const name = $(button).data("name");
  const maxQuantity = $(button).data("quantity");
  const unit = $(button).data("unit");
  const expirationDate = $(button).data("expiration");

  // Populate fields in the Remove Item modal
  $("#removeItemName").val(name);
  $("#removeItemUnit").val(unit);
  $("#removeItemExpiration").val(expirationDate);
  $("#removeItemQuantity").attr("max", maxQuantity).val(""); // Clear the input field
  $("#removeItemQuantityLabel").text(`Enter quantity to remove (Max: ${maxQuantity})`);

  // Store current item details for confirmation
  $("#removeItemModal").data("item", { name, maxQuantity, unit, expirationDate });

  // Show the modal
  $("#removeItemModal").modal("show");
}

// Confirm and process the removal
function confirmRemoveItem() {
  const { name, maxQuantity, unit, expirationDate } = $("#removeItemModal").data("item");
  const quantityToRemove = parseInt($("#removeItemQuantity").val(), 10);

  // Validate quantity
  if (isNaN(quantityToRemove) || quantityToRemove <= 0 || quantityToRemove > maxQuantity) {
    alert("Please enter a valid amount to remove.");
    return;
  }

  const username = getUsername();
  if (!username) {
    alert("You must be logged in.");
    return;
  }

  if (quantityToRemove === maxQuantity) {
    // Call the remove endpoint if the quantity to remove matches the max quantity
    const data = {
      username,
      name,
    };

    apiCall("http://localhost:8000/remove_fridge_item", data, "POST", function (err, response) {
      if (err) {
        alert("Failed to remove item: " + (err.error || err.statusText));
      } else {
        alert("Item removed successfully!");
        fetchFridgeItems(); // Refresh the fridge items after removal
        $("#removeItemModal").modal("hide"); // Close the modal
      }
    });
  } else {
    // Otherwise, update the item with the negative quantity
    const data = {
      username,
      name,
      quantity: -Math.abs(quantityToRemove), // Negative quantity for removal
      unit,
      location: "Fridge",
      expiration_date: expirationDate,
    };

    apiCall("http://localhost:8000/add_fridge_item", data, "POST", function (err, response) {
      if (err) {
        alert("Failed to update item: " + (err.error || err.statusText));
      } else {
        alert("Item updated successfully!");
        fetchFridgeItems(); // Refresh the fridge items after update
        $("#removeItemModal").modal("hide"); // Close the modal
      }
    });
  }
}


  
  // Add a new fridge item
  function addFridgeItem() {
    const username = getUsername();
    if (!username) {
      alert("You must be logged in to add items.");
      return;
    }
  
    const itemName = $("#itemName").val();
    const itemQuantity = $("#itemQuantity").val();
    const itemUnit = $("#itemUnit").val();
    const itemLocation = $("#itemLocation").val();
    const expirationDate = $("#expirationDate").val();
  
    if (!itemName || !itemQuantity || !itemUnit || !itemLocation || !expirationDate) {
      alert("All fields are required.");
      return;
    }
    if (itemQuantity <= 0) {
      alert("Quantity must be greater than 0.");
      return;
    }
    const data = {
      username,
      name: itemName,
      quantity: itemQuantity,
      unit: itemUnit,
      location: itemLocation,
      expiration_date: expirationDate,
    };
  
    apiCall("http://localhost:8000/add_fridge_item", data, "POST", function (err, response) {
      if (err) {
        alert("Failed to add item: " + (err.error || err.statusText));
      } else {
        alert("Item added successfully!");
        fetchFridgeItems(); // Refresh the fridge items after adding
      }
    });
  }  
  