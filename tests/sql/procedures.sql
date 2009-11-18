-- ALBUMS
CREATE OR REPLACE FUNCTION vtunes.insert_album(VARCHAR(100), VARCHAR(100), VARCHAR(100), DATE)
RETURNS INTEGER AS $$
DECLARE
    i_name	ALIAS FOR $1;
    i_genre	ALIAS FOR $2;
    i_artist    ALIAS FOR $3;
    i_date	ALIAS FOR $4;

    genre_id	INTEGER;
    artist_id	INTEGER;

    o_id        INTEGER;
BEGIN

    o_id := 0;

    SELECT id INTO genre_id FROM vtunes.genres WHERE name = i_genre;

    IF (genre_id IS NULL) THEN
	RAISE EXCEPTION '[VDB] No such genre!';

	RETURN 0;
    END IF;

    SELECT id INTO artist_id FROM vtunes.artists WHERE name = i_artist;

    IF (artist_id IS NULL) THEN
	RAISE EXCEPTION '[VDB] No such artist!';

	RETURN 0;
    END IF;

    INSERT INTO vtunes.albums(name, genre, artist, release_date) VALUES (i_name, genre_id, artist_id, i_date)
    RETURNING id INTO o_id;

    RETURN o_id;
END;
$$ LANGUAGE 'plpgsql';


-- TRACKS
CREATE OR REPLACE FUNCTION vtunes.insert_track(VARCHAR(100), VARCHAR(100), VARCHAR(100), INTEGER)
RETURNS INTEGER AS $$
DECLARE
    i_name	ALIAS FOR $1;
    i_artist	ALIAS FOR $2;
    i_album     ALIAS FOR $3;
    i_no	ALIAS FOR $4;

    artist_id	INTEGER;
    album_id	INTEGER;

    o_id        INTEGER;
BEGIN

    o_id := 0;

    SELECT id INTO artist_id FROM vtunes.artists WHERE name = i_artist;

    IF (artist_id IS NULL) THEN
	RAISE EXCEPTION '[VDB] No such artist!';

	RETURN 0;
    END IF;

    SELECT id INTO album_id FROM vtunes.albums WHERE name = i_album;

    IF (album_id IS NULL) THEN
	RAISE EXCEPTION '[VDB] No such album!';

	RETURN 0;
    END IF;

    INSERT INTO vtunes.tracks(name, artist, album, no) VALUES (i_name, artist_id, album_id, i_no)
    RETURNING id INTO o_id;

    RETURN o_id;
END;
$$ LANGUAGE 'plpgsql';

