cmake_minimum_required(VERSION 3.5)

# Define the component. SRCS needs to list all source files.
idf_component_register(
    SRCS 
        "src/PicoMQTT/client_wrapper.cpp"
        "src/PicoMQTT/client.cpp"
        "src/PicoMQTT/connection.cpp"
        "src/PicoMQTT/incoming_packet.cpp"
        "src/PicoMQTT/outgoing_packet.cpp"
        "src/PicoMQTT/print_mux.cpp"
        "src/PicoMQTT/publisher.cpp"
        "src/PicoMQTT/server.cpp"
        "src/PicoMQTT/subscriber.cpp"
        
    INCLUDE_DIRS 
        "src" # Includes the headers in the src folder
    
    # CRITICAL: Requires the Arduino framework (and probably other standard components)
    REQUIRES 
        "arduino-esp32"
        "driver" # Common dependency for I/O
        "lwip"   # Common dependency for networking
)