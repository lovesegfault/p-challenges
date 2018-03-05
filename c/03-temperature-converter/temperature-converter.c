//
// Created by bemeurer on 1/15/18.
//

#include "temperature-converter.h"

double fahrenheit_to_celsius(double temp) {
    return (temp - 32) * (5.0 / 9.0);
}

double celsius_to_fahrenheit(double temp) {
    return (temp * (9.0 / 5.0)) + 32;
}

double kelvin_to_celsius(double temp) {
    return (temp - 273.15);
}

double celsius_to_kelvin(double temp) {
    return (temp + 273.15);
}

double rankine_to_celsius(double temp) {
    return ((temp - 491.67) * (5.0 / 9.0));
}

double celsius_to_rankine(double temp) {
    return ((temp + 273.15) * (9.0 / 5.0));
}

double delisle_to_celsius(double temp) {
    return (100 - (temp * (2.0 / 3.0)));
}

double celsius_to_delisle(double temp) {
    return ((100 - temp) * (3.0 / 2.0));
}

double newton_to_celsius(double temp) {
    return (temp * (100.0 / 33.0));
}

double celsius_to_newton(double temp) {
    return (temp * (33.0 / 100.0));
}

double reaumur_to_celsius(double temp) {
    return (temp * (5.0 / 4.0));
}

double celsius_to_reaumur(double temp) {
    return (temp * (4.0 / 5.0));
}

double romer_to_celsius(double temp) {
    return ((temp - 7.5) * (40.0 / 21.0));
}

double celsius_to_romer(double temp) {
    return ((temp * (21.0 / 40.0)) + 7.5);
}


double convert_temperature(double temp, enum _temp_unit in_unit, enum _temp_unit out_unit) {
    double temp_celsius = 0;

    switch (in_unit) {
        case celsius:
            temp_celsius = temp;
            break;
        case fahrenheit:
            temp_celsius = fahrenheit_to_celsius(temp);
            break;
        case kelvin:
            temp_celsius = kelvin_to_celsius(temp);
            break;
        case rankine:
            temp_celsius = rankine_to_celsius(temp);
            break;
        case delisle:
            temp_celsius = delisle_to_celsius(temp);
            break;
        case newton:
            temp_celsius = newton_to_celsius(temp);
            break;
        case reaumur:
            temp_celsius = reaumur_to_celsius(temp);
            break;
        case romer:
            temp_celsius = romer_to_celsius(temp);
            break;
    }

    switch (out_unit) {
        case celsius:
            return (temp_celsius);
        case fahrenheit:
            return (celsius_to_fahrenheit(temp_celsius));
        case kelvin:
            return (celsius_to_kelvin(temp_celsius));
        case rankine:
            return (celsius_to_rankine(temp_celsius));
        case delisle:
            return (celsius_to_delisle(temp_celsius));
        case newton:
            return (celsius_to_newton(temp_celsius));
        case reaumur:
            return (celsius_to_reaumur(temp_celsius));
        case romer:
            return (celsius_to_romer(temp_celsius));
    }
    return temp_celsius;
}