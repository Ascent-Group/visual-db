-- LOCATIONS
CREATE OR REPLACE FUNCTION vtunes.check_location()
RETURNS trigger AS $$
DECLARE
    master_location INTEGER;
BEGIN
    IF (TG_OP = 'INSERT' OR TG_OP = 'UPDATE') THEN

	IF (NEW.master IS NOT NULL) THEN
	    SELECT id INTO master_location FROM vtunes.locations WHERE id = NEW.master;

	    IF (master_location IS NULL) THEN
		RAISE EXCEPTION '[VDB] Invalid master location!';

		RETURN NULL;
	    ELSE
		RETURN NEW;
	    END IF;

	ELSE
	    RETURN NEW;
	END IF;
    END IF;

    RETURN NULL;
END;
$$ LANGUAGE 'plpgsql';

CREATE TRIGGER locations_biu
BEFORE INSERT OR UPDATE ON vtunes.locations
FOR EACH ROW EXECUTE PROCEDURE vtunes.check_location();


-- ALBUMS
CREATE OR REPLACE FUNCTION vtunes.check_release_date()
RETURNS trigger AS $$
BEGIN
    IF (TG_OP = 'INSERT' OR TG_OP = 'UPDATE') THEN
	IF (NEW.release_date > now()) THEN
	    RAISE EXCEPTION '[VDB] You cannot predict the future, can you?';
	    RETURN NULL;
	ELSE
	    RETURN NEW;
	END IF;
    END IF;
END;
$$ LANGUAGE 'plpgsql';

CREATE TRIGGER albums_biu
BEFORE INSERT OR UPDATE ON vtunes.albums
FOR EACH ROW EXECUTE PROCEDURE vtunes.check_release_date();
