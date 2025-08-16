FROM ubuntu:latest

WORKDIR /app

COPY src/server.cpp .

RUN apt-get update && \
    apt-get install -y build-essential && \
    g++ server.cpp -o server

EXPOSE 8080

CMD ["./server"]
