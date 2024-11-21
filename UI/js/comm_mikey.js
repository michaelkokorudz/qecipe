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
 // Fetch fridge items and populate the table
function fetchFridgeItems() {
    const username = getUsername();
    if (!username) {
      alert("You must be logged in to view fridge items.");
      return;
    }
  
    apiCall("http://localhost:8000/get_fridge_items", { username }, "POST", function (err, response) {
      if (err) {
        alert("Failed to fetch fridge items: " + (err.error || err.statusText));
      } else {
        populateFridgeTable(response);
      }
    });
  }
  
  // Populate fridge items in the table
  function populateFridgeTable(items) {
    const tableBody = $("#inventoryTableBody");
    tableBody.empty(); // Clear the existing table rows
  
    const rows = items.split("\n").map((item) => {
      // Assuming API returns items as "name|quantity|unit|location|expiration"
      const [name, quantity, unit, location, expiration] = item.split("|").map((val) => val.trim());
  
      if (name) {
        return `
          <tr>
            <td>${name}</td>
            <td>${quantity}</td>
            <td>${unit}</td>
            <td>${location}</td>
            <td>${expiration}</td>
            <td class="text-right">No Actions Yet</td>
          </tr>
        `;
      }
      return "";
    });
  
    tableBody.append(rows.join(""));
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
  
  // Remove a fridge item
  function removeFridgeItem(itemName) {
    const username = getUsername();
    if (!username) {
      alert("You must be logged in to remove items.");
      return;
    }
  
    const data = { username, name: itemName };
  
    apiCall("http://localhost:8000/remove_fridge_item", data, "POST", function (err, response) {
      if (err) {
        alert("Failed to remove item: " + (err.error || err.statusText));
      } else {
        alert("Item removed successfully!");
        fetchFridgeItems(); // Refresh the fridge items after removing
      }
    });
  }
  