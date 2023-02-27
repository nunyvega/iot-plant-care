<?php

// Set the configuration values for the database
$dbConfig = [
    'host' => 'localhost',
    'name' => 'fxuntumy_iot_project',
    'user' => 'fxuntumy_nunyvega',
    'pass' => 'fxuntumy_!!',
];

// Set the API key value and initialize the apiKey variable
$apiKeyValue = 'bCsDN4xzG82NJRbJ9G222';
$apiKey = '';

// Check if the request method is POST, otherwise bail out
if ($_SERVER['REQUEST_METHOD'] === 'POST') {

    // Sanitize the API key from the POST request and check its value
    $apiKey = sanitize($_POST['api_key'] ?? '');
    if ($apiKey === $apiKeyValue) {

        // If the request contains a photo, save it to a file
        // This means that the communication is coming from ESP32 CAM
        if ($_POST['photo']) {
            error_log('img received');
            $imageData = ($_POST['photo']);
            $filename = 'camera.jpg';
            $filepath = 'uploads/' . $filename;
            $return_value = file_put_contents($filepath, $imageData);
        }

        // If the request contains moisture data, save the data to the database
        // This is a simple way to verify that the communication comes from ESP8266
        if ($_POST['moisture']) {
            $moisture = sanitize($_POST['moisture']);
            $humidity = sanitize($_POST['humidity']);
            $airTemperature = sanitize($_POST['airTemperature']);
            $soilTemperature = sanitize($_POST['soilTemperature']);

            // Create a connection to the database
            $conn = new mysqli($dbConfig['host'], $dbConfig['user'], $dbConfig['pass'], $dbConfig['name']);

            // Check if the connection was successful
            if ($conn->connect_error) {
                die('The connection failed. Error: ' . $conn->connect_error);
            }

            // Insert the plant data into the Sensor table
            $sql = "INSERT INTO Sensor (moisture, humidity, airTemperature, soilTemperature) VALUES ('$moisture', '$humidity', '$airTemperature', '$soilTemperature')";

            // If the query was unsuccessful, log an error
            if (!$conn->query($sql) === true) {
                error_log('Something went wrong. Error: ' . $sql . '<br>' . $conn->error);
            }

            // Close the database connection
            $conn->close();
        }
    } else {
        // If the API key is incorrect, log an error
        error_log('Incorrect API key');
    }
}

// Function to sanitize input data
function sanitize($data){
    $data = trim($data);
    $data = stripslashes($data);
    $data = htmlspecialchars($data);
    return $data;
}
