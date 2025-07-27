#include <iostream>
#include "connectors/oanda_connector.h"

int main() {
    // Replace with your actual API key and account ID for testing
    std::string api_key = "9e406b9a85efc53a6e055f7a30136e8e-3ef8b49b63d878ee273e8efa201e1536";
    std::string account_id = "101-001-31229774-001";

    sep::connectors::OandaConnector connector(api_key, account_id, true);

    if (!connector.initialize()) {
        std::cerr << "Failed to initialize OandaConnector: " << connector.getLastError() << std::endl;
        return 1;
    }

    std::cout << "OandaConnector initialized successfully." << std::endl;

    // Setup 48-hour sample data for EUR_USD
    connector.setupSampleData("EUR_USD", "M1", "eur_usd_m1_48h.json");

    if (connector.hasError()) {
        std::cerr << "Error setting up sample data: " << connector.getLastError() << std::endl;
        return 1;
    }

    std::cout << "Sample data setup complete." << std::endl;

    connector.shutdown();

    return 0;
}
