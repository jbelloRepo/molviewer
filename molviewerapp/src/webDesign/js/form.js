alert("Add and delete elements here!")
// 1. Form Submission
$(document).on("click", "#submit-btn", function (e) {
    e.preventDefault(); // Prevent default form submission behavior

    var elementNumber = $("#element-number").val();
    var elementCode = $("#element-code").val();
    var elementName = $("#element-name").val();
    var color1 = $("#color1").val();
    var color2 = $("#color2").val();
    var color3 = $("#color3").val();
    var radius = $("#radius").val();

    // Send a POST request to the server to process the form data
    $.ajax({
        url: "/form",
        type: "POST",
        data: {
            elementNumber: elementNumber,
            elementCode: elementCode,
            elementName: elementName,
            color1: color1,
            color2: color2,
            color3: color3,
            radius: radius
        },
        success: function (response) {
            alert("Form Submitted!")
            console.log(response); // Log the response to the console
            // Do something with the response if necessary
        },
        error: function (error) {
            alert("There was an error submitting the form")
            console.error(error);
        }
    });
});
// 2. View Element List button
$(document).ready(function () {
    // Add a click event listener to the "View Element List" button
    $("#view-btn").on("click", function () {
        // Make an AJAX request to the server to get the list of elements
        $.ajax({
            url: "/elements",
            type: "GET",
            dataType: "json",
            success: function (elements) {
                // Clear any existing rows in the table
                $("#element-table-body").empty();

                // Loop through the list of elements and add a row to the table for each one
                $.each(elements, function (index, element) {
                    var row = $("<tr></tr>");
                    row.append($("<td></td>").text(element.elementNumber));
                    row.append($("<td></td>").text(element.elementCode));
                    row.append($("<td></td>").text(element.elementName));
                    row.append($("<td></td>").text(element.color1));
                    row.append($("<td></td>").text(element.color2));
                    row.append($("<td></td>").text(element.color3));
                    row.append($("<td></td>").text(element.radius));
                    row.append($("<td></td>").html("<button class='delete-btn' data-id='" + element.id + "'>Delete</button>"));
                    $("#element-table-body").append(row);
                });
            },
            error: function (error) {
                console.error(error);
            }
        });
    });
});
// 3. Delete element from list
$(document).on("click", ".delete-btn", function () {
    // Get the ID of the element to delete
    var rowId = $(this).data("id");

    // Store the "this" reference in a variable to use inside the AJAX success callback
    var self = this;

    // Make an AJAX request to the server to delete the element
    $.ajax({
        url: "/rowDELETE",
        type: "POST",
        data: {
            id: rowId
        },
        dataType: "json",
        success: function (data) {
            if (data.status == "success") {
                // Remove the corresponding row from the table
                $(self).closest("tr").remove();

                // Alert the user that the row has been deleted
                alert(data.message);

                // Refresh the element list to show the updated element list
                $("#view-btn").click();
            } else {
                // Display an error message
                alert(data.message);
            }
        },
        error: function (error) {
            console.error(error);
        }
    });
});



// Get the form element
// const form = document.getElementById('add-element-form');

// // Add a submit event listener to the form
// form.addEventListener('submit', function (event) {
//     // Prevent the form from submitting
//     event.preventDefault();

//     // Get the input fields
//     const elementNumberInput = document.getElementById('element-number');
//     const elementCodeInput = document.getElementById('element-code');
//     const elementNameInput = document.getElementById('element-name');
//     const color1Input = document.getElementById('color1');
//     const color2Input = document.getElementById('color2');
//     const color3Input = document.getElementById('color3');
//     const radiusInput = document.getElementById('radius');

//     // Define a variable to store the error message
//     let errorMessage = '';

//     // Check each input field for any errors
//     if (elementNumberInput.value === '') {
//         errorMessage += 'Please enter an element number.\n';
//     } else if (isNaN(elementNumberInput.value)) {
//         errorMessage += 'Element number must be a number.\n';
//     }
//     if (elementCodeInput.value === '') {
//         errorMessage += 'Please enter an element code.\n';
//     } else if (elementCodeInput.value.length !== 2) {
//         errorMessage += 'Element code must be 2 characters.\n';
//     }
//     if (elementNameInput.value === '') {
//         errorMessage += 'Please enter an element name.\n';
//     }
//     if (radiusInput.value === '') {
//         errorMessage += 'Please enter a radius.\n';
//     } else if (isNaN(radiusInput.value)) {
//         errorMessage += 'Radius must be a number.\n';
//     }

//     // If there are any errors, display the error message and prevent the form from submitting
//     if (errorMessage !== '') {
//         alert(errorMessage);
//         return false;
//     }

//     // If there are no errors, allow the form to submit
//     form.submit();
// });


// // Add a submit event listener to the form
// form.addEventListener('submit', function(event) {
//     // Check if the form is valid
//     if (form.checkValidity() === false) {
//         event.preventDefault();
//         event.stopPropagation();
//     }

//     // Add bootstrap validation classes to the form controls
//     form.classList.add('was-validated');
// });


// $(document).on("click", "#submit-btn", function (e) {
//     e.preventDefault(); // Prevent default form submission behavior

//     var form = $("#add-element-form")[0];
//     var formData = new FormData(form);

//     // Send a POST request to the server to process the form data
//     $.ajax({
//         url: "/form",
//         type: "POST",
//         data: formData,
//         contentType: false,
//         processData: false,
//         success: function (response) {
//             console.log(response); // Log the response to the console
//             // Do something with the response if necessary
//         },
//         error: function (error) {
//             console.error(error);
//         }
//     });
// });


    const form = document.querySelector('#add-element-form');
    const elementNumberInput = form.querySelector('#element-number');
    const elementCodeInput = form.querySelector('#element-code');
    const elementNameInput = form.querySelector('#element-name');
    const radiusInput = form.querySelector('#radius');

    form.addEventListener('submit', (event) => {
        if (!form.checkValidity()) {
            event.preventDefault();
            event.stopPropagation();
        }
        form.classList.add('was-validated');
    });

    elementNumberInput.addEventListener('input', (event) => {
        if (elementNumberInput.validity.patternMismatch) {
            elementNumberInput.setCustomValidity('Element number must be a 3-digit number.');
        } else {
            elementNumberInput.setCustomValidity('');
        }
    });

    elementCodeInput.addEventListener('input', (event) => {
        if (elementCodeInput.validity.patternMismatch) {
            elementCodeInput.setCustomValidity('Element code must be a 2-character code.');
        } else {
            elementCodeInput.setCustomValidity('');
        }
    });

    elementNameInput.addEventListener('input', (event) => {
        if (elementNameInput.validity.patternMismatch) {
            elementNameInput.setCustomValidity('Element name must be a string of up to 20 characters.');
        } else {
            elementNameInput.setCustomValidity('');
        }
    });

    radiusInput.addEventListener('input', (event) => {
        if (radiusInput.validity.patternMismatch) {
            radiusInput.setCustomValidity('Radius must be a number.');
        } else {
            radiusInput.setCustomValidity('');
        }
    });

