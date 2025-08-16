FROM ubuntu:latest

WORKDIR /app

COPY src/client.cpp .

RUN apt-get update && \
    apt-get install -y build-essential && \
    g++ client.cpp -o client -pthread

CMD ["./client"]
