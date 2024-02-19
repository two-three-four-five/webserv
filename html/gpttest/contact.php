#!/usr/bin/php
<?php
header("Content-type: text/html");

if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $name = htmlspecialchars($_POST["name"]);
    $email = htmlspecialchars($_POST["email"]);
    $message = htmlspecialchars($_POST["message"]);

    // Perform any additional processing or validation as needed
    // For now, let's just display the submitted data
    echo "<h2>Form Submitted Successfully!</h2>";
    echo "<p>Name: $name</p>";
    echo "<p>Email: $email</p>";
    echo "<p>Message: $message</p>";
} else {
    // If the form is not submitted, redirect to the contact page
    header("Location: contact.html");
    exit();
}
?>
