-- ARTISTS
INSERT INTO vtunes.artists (id, name) VALUES (0, 'Unknown');
INSERT INTO vtunes.artists (id, name) VALUES (1, 'Children of Bodom');
INSERT INTO vtunes.artists (id, name) VALUES (2, 'Metallica');
INSERT INTO vtunes.artists (id, name) VALUES (3, 'Bon Jovi');
INSERT INTO vtunes.artists (id, name) VALUES (4, 'Michael Jackson');
INSERT INTO vtunes.artists (id, name) VALUES (5, 'Eminem');
INSERT INTO vtunes.artists (id, name) VALUES (6, 'Sarah Brightman');

-- GENRES
INSERT INTO vtunes.genres (id, name) VALUES (0, 'Unknown');
INSERT INTO vtunes.genres (id, name) VALUES (1, 'Classic');
INSERT INTO vtunes.genres (id, name) VALUES (2, 'Rap');
INSERT INTO vtunes.genres (id, name) VALUES (3, 'Pop');
INSERT INTO vtunes.genres (id, name) VALUES (4, 'Metal');
INSERT INTO vtunes.genres (id, name) VALUES (5, 'Opera');
INSERT INTO vtunes.genres (id, name) VALUES (6, 'Rock');

-- ALBUMS
SELECT vtunes.insert_album ('Something wild', 'Metal', 'Children of Bodom', '01-01-1998');
SELECT vtunes.insert_album ('Follow the Reaper', 'Metal', 'Children of Bodom', '01-01-2002');

SELECT vtunes.insert_album ('Load', 'Metal', 'Metallica', '01-01-1996');
SELECT vtunes.insert_album ('St. Anger', 'Metal', 'Metallica', '01-01-2003');

-- TRACKS
SELECT vtunes.insert_track ('Deadnight Warrior', 'Children of Bodom', 'Something wild', 1);
SELECT vtunes.insert_track ('In The Shadows', 'Children of Bodom', 'Something wild', 2);
SELECT vtunes.insert_track ('Red Light In My Eyes, pt 1', 'Children of Bodom', 'Something wild', 3);
SELECT vtunes.insert_track ('Red Light In My Eyes, pt 2', 'Children of Bodom', 'Something wild', 4);
SELECT vtunes.insert_track ('Lake Bodom', 'Children of Bodom', 'Something wild', 5);
SELECT vtunes.insert_track ('The Nail', 'Children of Bodom', 'Something wild', 6);
SELECT vtunes.insert_track ('Touch Like Angel Of Death', 'Children of Bodom', 'Something wild', 7);

SELECT vtunes.insert_track ('Follow The Reaper', 'Children of Bodom', 'Follow the Reaper', 1);
SELECT vtunes.insert_track ('Bodom After Midnight', 'Children of Bodom', 'Follow the Reaper', 2);
SELECT vtunes.insert_track ('Children Of Decadence', 'Children of Bodom', 'Follow the Reaper', 3);
SELECT vtunes.insert_track ('Everytime I Die', 'Children of Bodom', 'Follow the Reaper', 4);
SELECT vtunes.insert_track ('Mask Of Sanity', 'Children of Bodom', 'Follow the Reaper', 5);
SELECT vtunes.insert_track ('Taste Of My Scythe', 'Children of Bodom', 'Follow the Reaper', 6);
SELECT vtunes.insert_track ('Hate Me!', 'Children of Bodom', 'Follow the Reaper', 7);
SELECT vtunes.insert_track ('Northern Comfort', 'Children of Bodom', 'Follow the Reaper', 8);
SELECT vtunes.insert_track ('Kissing The Shadows', 'Children of Bodom', 'Follow the Reaper', 9);

SELECT vtunes.insert_track ('Until It Sleeps', 'Metallica', 'Load', 1);
SELECT vtunes.insert_track ('King Nothing', 'Metallica', 'Load', 2);
SELECT vtunes.insert_track ('Hero of the Day', 'Metallica', 'Load', 3);

-- LOCATIONS
INSERT INTO vtunes.locations (id, name) VALUES (0, 'Unknown');

INSERT INTO vtunes.locations (id, name) VALUES (1, 'Germany');
INSERT INTO vtunes.locations (id, name, master) VALUES (2, 'Berlin', 1);
INSERT INTO vtunes.locations (id, name, master) VALUES (3, 'Dresden', 1);

INSERT INTO vtunes.locations (id, name) VALUES (4, 'China');
INSERT INTO vtunes.locations (id, name, master) VALUES (5, 'Hong Kong', 4);
INSERT INTO vtunes.locations (id, name, master) VALUES (6, 'Nanking', 4);

-- USERS
INSERT INTO vtunes.users (id, name, location) VALUES (1, 'Dieter Krause', 2);
INSERT INTO vtunes.users (id, name, location) VALUES (2, 'Kwan Tung', 6);

-- PLAYLISTS
INSERT INTO vtunes.playlists (id, track, "user") VALUES (1, 1, 1);
INSERT INTO vtunes.playlists (id, track, "user") VALUES (1, 2, 1);
INSERT INTO vtunes.playlists (id, track, "user") VALUES (1, 3, 1);
INSERT INTO vtunes.playlists (id, track, "user") VALUES (1, 4, 1);
INSERT INTO vtunes.playlists (id, track, "user") VALUES (2, 5, 2);
INSERT INTO vtunes.playlists (id, track, "user") VALUES (2, 6, 2);
INSERT INTO vtunes.playlists (id, track, "user") VALUES (2, 7, 2);
INSERT INTO vtunes.playlists (id, track, "user") VALUES (2, 8, 2);
