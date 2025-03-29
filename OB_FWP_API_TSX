// 🌐 TSX (React with TypeScript JSX)
import React, { useEffect } from 'react';
import axios from 'axios';

const FoodWasteFetcher: React.FC = () => {
  useEffect(() => {
    const fetchData = async () => {
      try {
        const loginResponse = await axios.post('https://foodwaste-api-iz3pu.ondigitalocean.app/login', {
          email: 'your_email',
          password: 'your_password'
        });

        const token = loginResponse.data.token;
        if (!token) throw new Error("Login succeeded but no token returned");

        const dataResponse = await axios.post(
          'https://foodwaste-api-iz3pu.ondigitalocean.app/getCurrentFoodwasteData_by_machine_name',
          { machine_name: 'your_machine_name' },
          { headers: { Authorization: `Bearer ${token}` } }
        );

        console.log("Data received:", dataResponse.data);
        // Optional: download JSON
        const blob = new Blob([JSON.stringify(dataResponse.data, null, 2)], { type: 'application/json' });
        const link = document.createElement('a');
        link.href = URL.createObjectURL(blob);
        link.download = 'response.json';
        link.click();
      } catch (error) {
        console.error("Error:", error);
      }
    };

    fetchData();
  }, []);

  return <div>Fetching food waste data...</div>;
};

export default FoodWasteFetcher;
