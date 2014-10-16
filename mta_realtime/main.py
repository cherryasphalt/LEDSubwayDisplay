import mta_realtime, json, os, time
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

while True:
	feed = mta.get_by_id([259])

	mn_bound = feed[0]["N"][0]["time"].replace(tzinfo=None) - datetime.now()
	bk_bound = feed[0]["S"][0]["time"].replace(tzinfo=None) - datetime.now()

	mn_next_min = int(mn_bound.total_seconds() / 60) % 60
	bk_next_min = int(bk_bound.total_seconds() / 60) % 60

	print "MNHTN\n{min} min".format(min=mn_next_min)
	print "BKLYN\n{min} min".format(min=bk_next_min)

	time.sleep(30)	
