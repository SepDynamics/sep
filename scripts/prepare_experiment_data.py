import json

# --- Configuration ---

# 1. The master data file to split
input_file_path = 'Testing/OANDA/sample_48h.json'

# 2. The names for your output files
train_file_path = 'Testing/OANDA/O-train-1.json'
test_file_path = 'Testing/OANDA/O-test-2.json'

# 3. The split ratio for the training set.
#    - Set to 0.5 for a 50/50 split ("exactly in 2 parts").
#    - Set to 0.8 for a standard 80% train / 20% test split.
#    - Set to 0.7 for a 70% train / 30% test split.
train_split_ratio = 0.5 

# --- Main Script Logic ---

def split_file_for_ml(ratio):
    """
    Splits a time-series JSON file chronologically into a training set and a testing set.
    """
    print(f"Starting data split process...")
    print(f"Input file: {input_file_path}")
    print(f"Split ratio: {ratio*100:.0f}% for training, {(1-ratio)*100:.0f}% for testing.")
    
    try:
        # Load the entire JSON data from the input file
        with open(input_file_path, 'r') as f:
            data = json.load(f)

        # Check if the data is nested under a 'candles' key or is a direct list
        is_nested = isinstance(data, dict) and 'candles' in data
        records = data['candles'] if is_nested else data

        if not isinstance(records, list):
            print("Error: The JSON file does not contain a list of records.")
            return

        total_records = len(records)
        if total_records == 0:
            print("Error: No records found in the file.")
            return

        # Calculate the index at which to split the data
        split_index = int(total_records * ratio)

        # Split the list of records into two parts
        train_records = records[:split_index]
        test_records = records[split_index:]

        print("-" * 30)
        print(f"Total records found: {total_records}")
        print(f"Splitting at index: {split_index}")
        print(f"Training set size: {len(train_records)} records")
        print(f"Testing set size:  {len(test_records)} records")
        print("-" * 30)

        # Save the training data
        print(f"Writing training data to '{train_file_path}'...")
        with open(train_file_path, 'w') as f:
            # Preserve the original structure (nested or flat)
            output_data = {"candles": train_records} if is_nested else train_records
            json.dump(output_data, f, indent=2)

        # Save the testing data
        print(f"Writing testing data to '{test_file_path}'...")
        with open(test_file_path, 'w') as f:
            # Preserve the original structure (nested or flat)
            output_data = {"candles": test_records} if is_nested else test_records
            json.dump(output_data, f, indent=2)
            
        print("\nScript finished successfully!")

    except FileNotFoundError:
        print(f"Error: The file was not found at '{input_file_path}'")
    except json.JSONDecodeError:
        print(f"Error: The file '{input_file_path}' is not a valid JSON file.")
    except Exception as e:
        print(f"An unexpected error occurred: {e}")

# Run the function with the configured ratio
if __name__ == "__main__":
    split_file_for_ml(train_split_ratio)