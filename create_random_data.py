import os

# Create the directory if it doesn't exist
os.makedirs("assets/test_data", exist_ok=True)

# Create a random byte sequence
data = os.urandom(1200)  # 1200 random bytes

# Write the data to a binary file
with open("assets/test_data/random_data.bin", "wb") as f:
    f.write(data)

print("random_data.bin created successfully.")