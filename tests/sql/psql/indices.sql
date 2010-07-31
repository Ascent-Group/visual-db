-- ARTISTS
CREATE UNIQUE INDEX ind_artists ON vtunes.artists (name);

-- ALBUMS
CREATE INDEX  ind_albums ON vtunes.albums USING hash (name);

-- TRACKS
CREATE INDEX  ind_tracks ON vtunes.tracks USING hash (name);

-- USERS
CREATE UNIQUE INDEX ind_users ON vtunes."users" (name);

-- LOCATIONS
CREATE INDEX  ind_locations ON vtunes.locations USING hash (name);
