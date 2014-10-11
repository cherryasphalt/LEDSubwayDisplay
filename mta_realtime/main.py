import mta_realtime, json, os
from datetime import datetime



def default(obj):
    try:
        if isinstance(obj, datetime):
            return obj.isoformat()
        iterable = iter(obj)
    except TypeError:
        pass
    else:
        return list(iterable)
    return json.dumps(obj)

mta = mta_realtime.MtaSanitizer(
    os.environ['MTA_KEY'],
    "stations.json",
    10,
    30,
    60,
    True)

feed = mta.get_by_id([259])

delta = feed[0]["N"][0]["time"].replace(tzinfo=None) - datetime.now()

minutes_until_next_train = int(delta.total_seconds() / 60) % 60

print "{min} min".format(min=minutes_until_next_train)