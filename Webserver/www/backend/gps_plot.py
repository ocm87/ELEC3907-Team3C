import folium

def gen_gps_html (latitude: float, longitude: float):
    if (latitude == 0 or longitude == 0):
        return
    map=folium.Map(location=[latitude,longitude], zoom_start=14.6)
    map.add_child(folium.Marker(location=[latitude,longitude],popup='Current Location',icon=folium.Icon(color='red')))
    map.save("/var/www/html/gps_location.html")
    return
