PHONY := all build up logs down clean

all: build up

build:
	docker compose build

up:
	docker compose up -d

logs:
	docker compose logs

down:
	docker compose down

clean:
	docker compose down --rmi all --volumes --remove-orphans
	@echo "All Docker resources cleaned."
