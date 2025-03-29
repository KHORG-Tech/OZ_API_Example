<?php

$loginUrl = 'https://foodwaste-api-iz3pu.ondigitalocean.app/login';
$dataUrl = 'https://foodwaste-api-iz3pu.ondigitalocean.app/getCurrentFoodwasteData_by_machine_name';

// Step 1: Login
$loginPayload = json_encode([
    'email' => 'your_email',
    'password' => 'your_password'
]);

$ch = curl_init($loginUrl);
curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
curl_setopt($ch, CURLOPT_HTTPHEADER, ['Content-Type: application/json']);
curl_setopt($ch, CURLOPT_POSTFIELDS, $loginPayload);

$loginResponse = curl_exec($ch);
curl_close($ch);

$loginData = json_decode($loginResponse, true);
$token = $loginData['token'] ?? null;

if (!$token) {
    die("Login failed or token not returned\n");
}

echo "Logged in. Token: $token\n";

// Step 2: Get foodwaste data
$dataPayload = json_encode([
    'machine_name' => 'your_machine_name'
]);

$ch = curl_init($dataUrl);
curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
curl_setopt($ch, CURLOPT_HTTPHEADER, [
    'Content-Type: application/json',
    'Authorization: Bearer ' . $token
]);
curl_setopt($ch, CURLOPT_POSTFIELDS, $dataPayload);

$dataResponse = curl_exec($ch);
curl_close($ch);

echo "Data received\n";

file_put_contents('response.json', $dataResponse);
echo "Saved to response.json\n";

?>
