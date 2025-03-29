const fs = require('fs');
const fetch = require('node-fetch');

(async () => {
  try {
    // Step 1: Login
    const loginResponse = await fetch('https://foodwaste-api-iz3pu.ondigitalocean.app/login', {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify({
        email: 'your_email',
        password: 'your_password'
      })
    });

    const loginData = await loginResponse.json();
    const token = loginData.token;

    if (!token) throw new Error('Login succeeded but no token returned');
    console.log('Logged in successfully. Token:', token);

    // Step 2: Fetch data
    const dataResponse = await fetch('https://foodwaste-api-iz3pu.ondigitalocean.app/getCurrentFoodwasteData_by_machine_name', {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json',
        'Authorization': `Bearer ${token}`
      },
      body: JSON.stringify({
        machine_name: 'your_machine_name'
      })
    });

    const data = await dataResponse.json();
    console.log('Data received:', data);

    // Step 3: Save response to JSON file
    fs.writeFileSync('response.json', JSON.stringify(data, null, 2));
    console.log('💾 Saved to response.json');

  } catch (error) {
    console.error('Error:', error.message);
  }
})();
