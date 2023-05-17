// 1. View list button
// Load the molecule list table when the view list button is clicked
$("#view-list-btn").on("click", function () {
  // Send a POST request to the server to retrieve the molecules data
  $.ajax({
    url: "/moleculelist",
    type: "POST",
    dataType: "json",
    success: function (response) {
      console.log(response); // Log the response to the console

      // Parse the JSON response and populate the table with the data
      var moleculeList = response;
      var tableBody = $("#molecule-table-body");
      tableBody.empty();
      for (var i = 0; i < moleculeList.length; i++) {
        var row = $("<tr></tr>");
        var idCell = $("<td></td>").text(moleculeList[i].id);
        var nameCell = $("<td></td>").html('<a href="#" class="molecule-link" data-id="' + moleculeList[i].id + '">' + moleculeList[i].name + '</a>');
        row.append(idCell, nameCell);
        tableBody.append(row);
      }

      // Show an alert with the response received from the server
      alert("Molecule list loaded successfully!");
    },
    error: function (error) {
      console.error(error);
    }
  });
});
// 2. Molecule links
// Add click event handler for molecule links
$(document).on("click", ".molecule-link", function (e) {
  e.preventDefault(); // Prevent default link behavior
  console.log('This prevention is working')

  var moleculeId = $(this).data("id"); // Get the molecule ID from data attribute

  // Set the ID of the rotate button to the molecule ID
  $("#rotate-button").data("id", moleculeId);


  // Send a POST request to the server to retrieve SVG file for the selected molecule
  $.ajax({
    url: "/doPOST",
    type: "POST",
    data: {
      id: moleculeId
    }, // Send molecule ID as data
    success: function (response) {
      console.log('Success')
      console.log(new XMLSerializer().serializeToString(response))
      console.log(response); // Log the response to the console

      // Serialize the XML document to a string
      var serializer = new XMLSerializer();
      var svgString = serializer.serializeToString(response);

      // Set the serialized SVG string as the inner HTML of the svg-container element
      $("#svg-container").html(svgString);

      // Enable the rotate button
      $("#rotate-button").prop("disabled", false);
      console.log('Sent to container')
    },
    error: function (error) {
      console.log("This is the error printing")
      console.error(error);
    }
  });
});
// 3. Rotate Button
// Add click event handler for rotate button
$(document).on("click", "#rotate-button", function (e) {
  e.preventDefault(); // Prevent default button behavior

  // Get the molecule ID from the rotate button data attribute
  var moleculeId = $(this).data("id");
  console.log("The molecule ID is: " + moleculeId);

  // Send a POST request to the server to perform rotation on the molecule
  $.ajax({
    url: "/rotate",
    type: "POST",
    data: {
      id: moleculeId
    }, // Send molecule ID as data
    success: function (response) {
      console.log('Rotate Success');
      // Do something with the response

      // Serialize the XML document to a string
      var serializer = new XMLSerializer();
      var svgString = serializer.serializeToString(response);

      // Set the serialized SVG string as the inner HTML of the svg-container element
      $("#svg-container").html(svgString);
    },
    error: function (error) {
      console.log("Rotate Error");
      console.error(error);
    }
  });
});