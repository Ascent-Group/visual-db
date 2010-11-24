-- ARTISTS
CREATE TABLE vtunes.artists (
    id              SERIAL PRIMARY KEY,
    name            VARCHAR(100) UNIQUE NOT NULL
);

-- GENRES
CREATE TABLE vtunes.genres (
    id              SERIAL PRIMARY KEY,
    name            VARCHAR(100) UNIQUE NOT NULL
);

-- ALBUMS
CREATE TABLE vtunes.albums (
    id              SERIAL PRIMARY KEY,
    name            VARCHAR(100) NOT NULL,
    genre       INTEGER REFERENCES vtunes.genres(id) ON UPDATE CASCADE ON DELETE CASCADE NOT NULL,
    artist      INTEGER REFERENCES vtunes.artists(id) ON UPDATE CASCADE ON DELETE CASCADE NOT NULL,
    release_date    DATE
);

-- TRACKS
CREATE TABLE vtunes.tracks (
    id              SERIAL PRIMARY KEY,
    name            VARCHAR(100) NOT NULL,
    artist      INTEGER REFERENCES vtunes.artists(id) ON UPDATE CASCADE ON DELETE CASCADE NOT NULL,
    album       INTEGER REFERENCES vtunes.albums(id) ON UPDATE CASCADE ON DELETE CASCADE NOT NULL,
    -- number of track in an album
    no          INTEGER
);

-- LOCATIONS
CREATE TABLE vtunes.locations (
    id              SERIAL PRIMARY KEY,
    name            VARCHAR(100) NOT NULL,
    master          INTEGER REFERENCES vtunes.locations(id) ON UPDATE CASCADE ON DELETE CASCADE
);

-- USERS
CREATE TABLE vtunes.users (
    id              SERIAL PRIMARY KEY,
    name            VARCHAR(100) UNIQUE NOT NULL,
    location        INTEGER REFERENCES vtunes.locations(id) ON UPDATE CASCADE ON DELETE CASCADE NOT NULL
);

-- PLAYLISTS
CREATE TABLE vtunes.playlists (
    id              INTEGER,
    track       INTEGER REFERENCES vtunes.tracks(id) ON UPDATE CASCADE ON DELETE CASCADE NOT NULL,
    "user"      INTEGER REFERENCES vtunes.users(id) ON UPDATE CASCADE ON DELETE CASCADE NOT NULL
);

-- table inheritance
CREATE TABLE vtunes.extended_playlists (
    expires     DATE
) INHERITS (vtunes.playlists);

-- composite PK
CREATE TABLE vtunes.user_alias (
    id      SERIAL,
    "user"  INTEGER REFERENCES vtunes.users(id) ON UPDATE CASCADE ON DELETE CASCADE NOT NULL,
    alias   VARCHAR(20) UNIQUE NOT NULL,
    PRIMARY KEY ( id, "user" )
);

