class Weather_Modeler(object):
    def __init__(self, links, weather_factor_table):
        self.links = links;
        self.weather_factor_table = weather_factor_table;

        """ Column numbers in link file format """
        self.link_free_flow_speed_column = 4
        self.link_capacity_column = 5
        self.link_type_column = 6

        """ get capacity factor """
        self.freeway_capacity_factor = weather_factor_table.get("capacity factor").get("freeway")    
        self.arterial_capacity_factor = weather_factor_table.get("capacity factor").get("arterial")    
        self.speed_borders = list()
        self.freeway_speed_factors = list()
        self.arterial_speed_factors = list()
        """ get free-flow speed factor """
        for factor_line in weather_factor_table.get("speed factor"):
            self.speed_borders.append(factor_line[0])
            self.freeway_speed_factors.append(factor_line[1].get('freeway'))
            self.arterial_speed_factors.append(factor_line[1].get('arterial'))

    def adjust_capacity(self, link):
        link_type = link[self.link_type_column]
        capacity = float(link[self.link_capacity_column])
        if link_type == 'FREEWAY' or link_type == 'EXPRESSWAY':
            factor = self.freeway_capacity_factor
        else:
            factor = self.arterial_capacity_factor
        capacity *= factor
        link[self.link_capacity_column] = str(capacity)
    
    def adjust_free_flow_speed(self, link):
        link_type = link[self.link_type_column]
        speed = float(link[self.link_free_flow_speed_column])
        speed_index = 0
        for speed_border in self.speed_borders:
            if speed > speed_border:
                speed_index += 1
                continue
            else:
                break
        speed_index = min(len(self.speed_borders) - 1, speed_index)
        if link_type == 'FREEWAY' or link_type == 'EXPRESSWAY':
            factor = self.freeway_speed_factors[speed_index]
        else:
            factor = self.arterial_speed_factors[speed_index]
        speed *= factor
        link[self.link_free_flow_speed_column] = str(speed)
    
    def adjust(self):
        for link in self.links:
            self.adjust_capacity(link)
            self.adjust_free_flow_speed(link)
