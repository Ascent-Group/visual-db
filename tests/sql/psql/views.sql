-- USERS' PLAYLISTS
CREATE VIEW vtunes.users_playlists AS
    SELECT
	u.name,
	p.track
    FROM
	vtunes.users u,
	vtunes.playlists p
    WHERE
	p.user = u.id;

-- ARTISTS' TRACKS
CREATE VIEW vtunes.artists_tracks AS
    SELECT
	ar.name AS Artist,
	al.name AS Album,
	t.no AS Number,
	t.name AS Track
    FROM
	vtunes.albums al,
	vtunes.artists ar,
	vtunes.tracks t
    WHERE
	ar.id = al.artist
	AND ar.id = t.artist
	AND al.id = t.album
    GROUP BY ar.name, al.name, t.no, t.name;
