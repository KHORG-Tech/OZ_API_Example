package main

import (
	"bytes"
	"encoding/json"
	"fmt"
	"io/ioutil"
	"net/http"
	"os"
)

func main() {
	loginURL := "https://foodwaste-api-iz3pu.ondigitalocean.app/login"
	dataURL := "https://foodwaste-api-iz3pu.ondigitalocean.app/getCurrentFoodwasteData_by_machine_name"

	// Step 1: Login
	loginPayload := map[string]string{
		"email":    "your_email",
		"password": "your_password",
	}
	loginBody, _ := json.Marshal(loginPayload)

	resp, err := http.Post(loginURL, "application/json", bytes.NewBuffer(loginBody))
	if err != nil {
		fmt.Println("Login request failed:", err)
		return
	}
	defer resp.Body.Close()

	var loginResult map[string]interface{}
	json.NewDecoder(resp.Body).Decode(&loginResult)

	token, ok := loginResult["token"].(string)
	if !ok || token == "" {
		fmt.Println("Login succeeded but token not returned.")
		return
	}

	fmt.Println("Logged in successfully. Token:", token)

	// Step 2: Get data
	dataPayload := map[string]string{
		"machine_name": "your_machine_name",
	}
	dataBody, _ := json.Marshal(dataPayload)

	req, _ := http.NewRequest("POST", dataURL, bytes.NewBuffer(dataBody))
	req.Header.Set("Content-Type", "application/json")
	req.Header.Set("Authorization", "Bearer "+token)

	client := &http.Client{}
	dataResp, err := client.Do(req)
	if err != nil {
		fmt.Println("Data request failed:", err)
		return
	}
	defer dataResp.Body.Close()

	body, _ := ioutil.ReadAll(dataResp.Body)
	fmt.Println("Data received")

	// Step 3: Save to file
	err = os.WriteFile("response.json", body, 0644)
	if err != nil {
		fmt.Println("Failed to save file:", err)
	} else {
		fmt.Println("Saved to response.json")
	}
}
