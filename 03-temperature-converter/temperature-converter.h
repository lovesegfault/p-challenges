//
// Created by bemeurer on 1/15/18.
//

#ifndef INC_02_TEMPERATURE_CONVERTER_TEMPERATURE_CONVERTER_H
#define INC_02_TEMPERATURE_CONVERTER_TEMPERATURE_CONVERTER_H

typedef enum _temp_unit{
    kelvin,
    celsius,
    fahrenheit,
    rankine,
    delisle,
    newton,
    reaumur,
    romer,
} temp_unit;

double convert_temperature(double, enum _temp_unit, enum _temp_unit);

#endif //INC_02_TEMPERATURE_CONVERTER_TEMPERATURE_CONVERTER_H
