import os

# Create the directory if it doesn't exist
os.makedirs("assets/test_data", exist_ok=True)

# Create a repetitive byte sequence
data = bytearray(range(12)) * 100  # Repeat the sequence 100 times

# Write the data to a binary file
with open("assets/test_data/repetitive_data.bin", "wb") as f:
    f.write(data)

print("repetitive_data.bin created successfully.")