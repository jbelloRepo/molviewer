alert('Allow for confirmation of successful upload before naming molecule')

// // add listener to molecule name input
// molNameInput.addEventListener('input', function (event) {
//     const invalidCharsRegex = /[\s~`!@#$%^&*()-_=+[\]{}\\|;:'",.<>/?]/g;
//     if (invalidCharsRegex.test(molNameInput.value)) {
//         molNameInput.setCustomValidity(
//             'Please enter a valid molecule name without spaces or special characters.');
//         submitBtn.disabled = true; // disable submit button
//     } else {
//         molNameInput.setCustomValidity('');
//         if (!uploadBtn.disabled) {
//             submitBtn.disabled = false; // enable submit button
//         }
//     }
// });

// // Submit Molecule name action
// const submitBtn = document.querySelector('#submit-btn');
// submitBtn.addEventListener('click', function (event) {
//     event.preventDefault(); // Prevent the form from submitting

//     // Your code for confirmation or validation before submitting the form
//     const confirmMsg = "Are you sure you want to submit the form?";
//     if (confirm(confirmMsg)) {
//         // If user confirms, submit the form
//         const form = document.querySelector('form');
//         form.submit();
//     }
// });

// // Add validation for molecule name input
// const molNameInput = document.getElementById('mol_name');
// molNameInput.addEventListener('input', function (event) {
//     const invalidCharsRegex = /[\s~`!@#$%^&*()-_=+[\]{}\\|;:'",.<>/?]/g;
//     if (invalidCharsRegex.test(molNameInput.value)) {
//         molNameInput.setCustomValidity(
//             'Please enter a valid molecule name without spaces or special characters.');
//     } else {
//         molNameInput.setCustomValidity('');

//     }
// });

// fileInput.addEventListener('change', function(event) {
//     const selectedFile = event.target.files[0];
//     const fileName = selectedFile.name;

//     if (!fileName.endsWith('.sdf')) {
//         // Show invalid feedback message
//         invalidFeedback.textContent = 'Please upload a file with an .sdf extension.';
//         form.classList.add('was-validated');
//         fileInput.setCustomValidity('invalid');
//     } else {
//         // Clear invalid feedback message
//         invalidFeedback.textContent = '';
//         form.classList.remove('was-validated');
//         fileInput.setCustomValidity('');
//     }
// });

// const fileInput = document.getElementById('sdf_file');
// const form = document.querySelector('form');
// // const invalidFeedback = document.querySelector('#sdf_file ~ .invalid-feedback');
// const invalidFeedback = document.querySelector('#step1 .invalid-feedback');

// fileInput.addEventListener('change', function(event) {
//     const selectedFile = event.target.files[0];
//     const fileType = selectedFile.type;
//     const fileName = selectedFile.name;

//     // alert(fileName);

//     if (!fileName.endsWith('.sdf')) {
//         // Show invalid feedback message
//         invalidFeedback.textContent = 'Please upload a file with an .sdf extension.';
//         form.classList.add('was-validated');
//         fileInput.setCustomValidity('invalid');
//     } else {
//         // Clear invalid feedback message
//         invalidFeedback.textContent = '';
//         form.classList.remove('was-validated');
//         fileInput.setCustomValidity('');
//     }
// });


// main 

// const fileInput = document.getElementById('sdf_file');
// const form = document.querySelector('form');
// const invalidFeedback = document.querySelector('#step1 .invalid-feedback');

// fileInput.addEventListener('change', function(event) {
//     const selectedFile = event.target.files[0];
//     const fileName = selectedFile.name;

//     if (!fileName.endsWith('.sdf')) {
//         // Show invalid feedback message
//         invalidFeedback.textContent = 'Please upload a file with an .sdf extension.';
//         form.classList.add('was-validated');
//         fileInput.setCustomValidity('invalid');
//     } else {
//         // Clear invalid feedback message
//         invalidFeedback.textContent = '';
//         form.classList.remove('was-validated');
//         fileInput.setCustomValidity('');
//     }
// });


const fileInput = document.getElementById('sdf_file'); // sdf file
const form = document.querySelector('form');
const invalidFeedback = document.querySelector('#sdf_file ~ .invalid-feedback'); //feedback
const uploadBtn = document.querySelector('button[type="submit"]'); //upload button
const molNameInput = document.getElementById('mol_name'); // molecule name input

// disable upload button initially
uploadBtn.disabled = true;
molNameInput.disabled = true;

// add listener to input
fileInput.addEventListener('change', function (event) {
    const selectedFile = event.target.files[0];
    const fileType = selectedFile.type;
    const fileName = selectedFile.name;

    // alert(fileName);

    //check for sdf
    if (!fileName.endsWith('.sdf')) {
        // Show invalid feedback message
        invalidFeedback.textContent = 'Please upload a file with an .sdf extension.';
        form.classList.add('was-validated');
        fileInput.setCustomValidity('invalid');
        uploadBtn.disabled = true; // disable upload button
        molNameInput.disabled = true; // disable molecule name input
    } else {
        // Clear invalid feedback message
        invalidFeedback.textContent = '';
        form.classList.remove('was-validated');
        fileInput.setCustomValidity('');
        molNameInput.disabled = false; // enable molecule name input
    }
});

// add listener to molecule name input
molNameInput.addEventListener('input', function (event) {
    const invalidCharsRegex = /[\s~`!@#$%^&*()-_=+[\]{}\\|;:'",.<>/?]/g;
    if (invalidCharsRegex.test(molNameInput.value)) {
        molNameInput.setCustomValidity(
            'Please enter a valid molecule name without spaces or special characters.');
        form.classList.add('was-validated'); // add class to form to trigger error message display
        uploadBtn.disabled = true; // disable upload button
    } else {
        molNameInput.setCustomValidity('');
        if (!fileInput.value || !fileInput.validity.valid) {
            uploadBtn.disabled = true; // disable upload button
        } else {
            uploadBtn.disabled = false; // enable upload button
        }
    }
});


// $(document).ready(function() {
//     $('form').submit(function(event) {
//       // prevent the form from submitting and reloading the page
//       event.preventDefault();
  
//       // add a "validating..." animation to the upload button
//       $('button[type="submit"]').html('<span class="spinner-border spinner-border-sm" role="status" aria-hidden="true"></span> Validating...').prop('disabled', true);
  
//       // submit the form data to the server using AJAX
//       $.ajax({
//         url: 'upload.php',
//         type: 'POST',
//         data: new FormData(this),
//         processData: false,
//         contentType: false,
//         success: function(response) {
//           // remove the animation and enable the upload button
//           $('button[type="submit"]').html('Upload').prop('disabled', false);
  
//           // display a success message to the user
//           alert('File uploaded successfully!');
//         },
//         error: function(jqXHR, textStatus, errorThrown) {
//           // remove the animation and enable the upload button
//           $('button[type="submit"]').html('Upload').prop('disabled', false);
  
//           // display an error message to the user
//           alert('Error uploading file: ' + textStatus + ' - ' + errorThrown);
//         }
//       });
//     });
//   });


//   // add a "validating..." animation to the upload button
// $('button[type="submit"]').click(function() {
//     // disable the upload button to prevent multiple clicks
//     $(this).prop('disabled', true);
//     // show the "validating..." animation
//     $(this).html('<span class="spinner-border spinner-border-sm" role="status" aria-hidden="true"></span> Validating...');
//     // wait for 10 seconds before showing the success message
//     setTimeout(function() {
//         // enable the upload button and show the success message
//         $('button[type="submit"]').prop('disabled', false).html('Upload');
//         $('.upload-feedback').html('File uploaded successfully');
//     }, 2000);
// });

// $('button[type="submit"]').click(function() {
//     // disable the upload button to prevent multiple clicks
//     $(this).prop('disabled', true);
//     // show the "validating..." animation
//     $(this).html('<span class="spinner-border spinner-border-sm" role="status" aria-hidden="true"></span> Validating...');
//     // wait for 10 seconds before showing the success message
//     setTimeout(function() {
//         // enable the upload button and show the success message
//         $('button[type="submit"]').prop('disabled', false).html('Upload');
//         $('.upload-feedback').html('File uploaded successfully');
//     }, 1000);
// });


// $('form').submit(function(e) {
//     e.preventDefault();
//     // add a "validating..." animation to the upload button
//     $('button[type="submit"]').prop('disabled', true).html('<span class="spinner-border spinner-border-sm" role="status" aria-hidden="true"></span> Validating...');
//     // wait for 2 seconds before showing the success message
//     setTimeout(function() {
//         // enable the upload button and show the success message
//         $('button[type="submit"]').prop('disabled', false).html('Upload');
//         $('.upload-feedback').html('File uploaded successfully');
//     }, 2000);
// });

// $('button[type="submit"]').click(function(event) {
//      // prevent the default form submission behavior
//      event.preventDefault();
//     // disable the upload button to prevent multiple clicks
//     $(this).prop('disabled', true);
//     // show the "validating..." animation
//     $(this).html('<span class="spinner-border spinner-border-sm" role="status" aria-hidden="true"></span> Validating...');
//     // make an AJAX request to the server
//     $.ajax({
//         url: '/success_message',
//         type: 'POST',
//         data: $('form').serialize(),
//         success: function(response) {
//             if (response.success) {
//                 // enable the upload button and show the success message
//                 $('button[type="submit"]').prop('disabled', false).html('Upload');
//                 $('.upload-feedback').html(response.message);
//             } else {
//                 // show the error message
//                 $('.upload-feedback').html(response.message);
//             }
//         },
//             // error: function() {
//             //     $('button[type="submit"]').prop('disabled', false).html('Upload');
//             //     $('.upload-feedback').html('An error occurred while processing the file');
//             // }
//     });
// });

