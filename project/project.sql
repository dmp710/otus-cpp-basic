CREATE TABLE users (
    user_id SERIAL PRIMARY KEY,
    email VARCHAR UNIQUE NOT NULL,
    hash VARCHAR NOT NULL
);

CREATE TABLE actions(
    user_id INT NOT NULL,
    task_name VARCHAR NOT NULL,
    description VARCHAR,
    time_start TIMESTAMP NOT NULL,
    time_end TIMESTAMP NOT NULL,
    CHECK (time_end > time_start),
    FOREIGN KEY (user_id) REFERENCES users(user_id)
);