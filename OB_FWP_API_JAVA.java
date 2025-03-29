import java.io.*;
import java.net.URI;
import java.net.http.*;
import java.nio.file.Files;
import java.nio.file.Paths;

public class FoodWasteAPIClient {

    public static void main(String[] args) throws Exception {
        String loginUrl = "https://foodwaste-api-iz3pu.ondigitalocean.app/login";
        String dataUrl = "https://foodwaste-api-iz3pu.ondigitalocean.app/getCurrentFoodwasteData_by_machine_name";

        HttpClient client = HttpClient.newHttpClient();

        // Step 1: Login
        String loginJson = """
            {
              "email": "your_email",
              "password": "your_password"
            }
            """;

        HttpRequest loginRequest = HttpRequest.newBuilder()
                .uri(URI.create(loginUrl))
                .header("Content-Type", "application/json")
                .POST(HttpRequest.BodyPublishers.ofString(loginJson))
                .build();

        HttpResponse<String> loginResponse = client.send(loginRequest, HttpResponse.BodyHandlers.ofString());

        String token = extractToken(loginResponse.body());
        if (token == null) {
            throw new RuntimeException("Login succeeded but no token returned.");
        }

        System.out.println("Token acquired");

        // Step 2: Fetch food waste data
        String dataJson = """
            {
              "machine_name": "your_machine_name"
            }
            """;

        HttpRequest dataRequest = HttpRequest.newBuilder()
                .uri(URI.create(dataUrl))
                .header("Content-Type", "application/json")
                .header("Authorization", "Bearer " + token)
                .POST(HttpRequest.BodyPublishers.ofString(dataJson))
                .build();

        HttpResponse<String> dataResponse = client.send(dataRequest, HttpResponse.BodyHandlers.ofString());

        System.out.println("Data received:");
        System.out.println(dataResponse.body());

        // Step 3: Save to response.json
        Files.write(Paths.get("response.json"), dataResponse.body().getBytes());
        System.out.println("💾 Saved to response.json");
    }

    private static String extractToken(String json) {
        // Naive token extractor for demo purposes. Use a proper JSON library (like Jackson/Gson) for production.
        int start = json.indexOf("\"token\":\"");
        if (start == -1) return null;
        start += 9;
        int end = json.indexOf("\"", start);
        return json.substring(start, end);
    }
}
