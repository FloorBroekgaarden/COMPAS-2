#include <iostream>
#include <stdarg.h>
#include <fstream>
#include <algorithm>
#include <cstring>
#include <math.h>
#include "utils.h"
#include "Rand.h"


/*
 * utility functions that don't belong in any class
 *
 */
namespace utils {

    /*
     * Announce COMPAS
     * 
     * Constructs and returns a splash string.  Prints string to stdout if required.
     *
     *
     * void SplashScreen(const bool p_Print)
     * 
     * @param   [IN]    p_Print             Boolean indicating whether splash string should be printed.  Default is TRUE
     * @return                              Splash string
     */
    std::string SplashScreen(const bool p_Print) {

        // Construct the splash string
        std::string splashString = "\nCOMPAS v" + 
                                   VERSION_STRING + 
                                   "\nCompact Object Mergers: Population Astrophysics and Statistics"
                                   "\nby Team COMPAS (http://compas.science/index.html)"
                                   "\nA binary star simulator\n";

        if (p_Print) std::cout << splashString << std::endl;    // print the splash string if required

        return splashString;                                    // return the splash string
    }


    /*
     * Compare floating-point numbers with tolerance
     *
     * Absolute and relative tolerance can be different - see constants.h
     * Set relative tolerance = 0.0 to always use absolute
     * Set absolute tolerance = 0.0 to always use relative
     * Set both to zero for no tolerance - or #undef COMPARE_WITH_TOLERANCE for performance
     *
     *
     * int Compare(const double p_X, const double p_Y)
     *
     * @param   [IN]    p_X                 Floating-point value to be compared
     * @param   [IN]    p_Y                 Floating-point value to be compared
     * @return                              Integer indicating result of comparison:
     *                                         -1 indicates p_X is less than p_Y
     *                                          0 indicates equality
     *                                          1 indicates p_X is greater than p_Y
     */
    int Compare(const double p_X, const double p_Y) {
    #ifdef COMPARE_WITH_TOLERANCE
        return (std::abs(p_X - p_Y) <= std::max(FLOAT_TOLERANCE_ABSOLUTE, FLOAT_TOLERANCE_RELATIVE * std::max(std::abs(p_X), fabs(p_Y)))) ? 0 : (p_X < p_Y ? -1 : 1);
    #else
        return (p_X == p_Y) ? 0 : (p_X < p_Y ? -1 : 1);
    #endif
    }


    /*
     * Determine if filename exists - input parameter is character array
     *
     *
     * bool FileExists(const char *p_Filename)
     *
     * @param   [IN]    p_Filename                  Filename to be checked - should be fully qualified
     * @return                                      Boolean indicating whether file exists
     */
    bool FileExists(const char *p_Filename) {
        std::ifstream ifile(p_Filename);
        return (bool)ifile;
    }


    /*
     * Determine if filename exists - input parameter is std::string
     *
     *
     * bool FileExists(const std::string& p_Filename)
     *
     * @param   [IN]    p_Filename                  Filename to be checked - should be fully qualified
     * @return                                      Boolean indicating whether file exists
     */
    bool FileExists(const std::string& p_Filename) {
        return FileExists(p_Filename.c_str());
    }


    /*
     * Upshift lowercase characters in string to uppercase
     *
     * This only works with ASCII data, but I think that's all we need
     *
     *
     * bool std::string ToUpper(std::string p_Str)
     *
     * @param   [IN]    p_Str                       String to be upshifted
     * @return                                      Upshifted string
     */
    std::string ToUpper(std::string p_Str) {
        std::transform(p_Str.begin(), p_Str.end(), p_Str.begin(), ::toupper);
        return p_Str;
    }


    /*
     * Downshift uppercase characters in string to lowercase
     *
     * This only works with ASCII data, but I think that's all we need
     *
     *
     * bool std::string ToLower(std::string p_Str)
     *
     * @param   [IN]    p_Str                       String to be downshifted
     * @return                                      Downshifted string
     */
    std::string ToLower(std::string p_Str) {
        std::transform(p_Str.begin(), p_Str.end(), p_Str.begin(), ::tolower);
        return p_Str;
    }


    /*
     * Case-insensitive comparison of strings
     *
     * This only works with ASCII data, but I think that's all we need
     * Note that std::string has an == operator to test for equality (actually calls std::strcmp)
     *
     *
     * bool Equals(std::string p_Str1, std::string p_Str2)
     *
     * @param   [IN]    p_Str1                      String to be compared
     * @param   [IN]    p_Str2                      String to be compared
     * @return                                      Boolean indicating equality (true = equal)
     */
    bool Equals(std::string p_Str1, std::string p_Str2) {
        std::transform(p_Str1.begin(), p_Str1.end(), p_Str1.begin(), ::tolower);
        std::transform(p_Str2.begin(), p_Str2.end(), p_Str2.begin(), ::tolower);
        return (std::strcmp(p_Str1.c_str(), p_Str2.c_str()) == 0);
    }


    /*
     * Pads string to specified length by prepending the string with "0"
     *
     * This only works with ASCII data, but I think that's all we need
     * Note that std::string has an == operator to test for equality (actually calls std::strcmp)
     *
     *
     * std::string PadLeadingZeros(const std::string p_Str, const std::size_t p_MaxLength)
     *
     * @param   [IN]    p_Str                       String to be padded with leading "0"s
     * @param   [IN]    p_MaxLength                 The required length of the resultant string compared
     * @return                                      String padded with leading "0"s - will be unchanged from input string if length alread >= required length
     */
    std::string PadLeadingZeros(const std::string p_Str, const std::size_t p_MaxLength) {
        return (p_Str.length() < p_MaxLength) ? std::string(p_MaxLength - p_Str.length(), '0') + p_Str : p_Str;
    }


    /*
     * Centre-justfies string to specified width by prepending and appending spaces.
     * Extra space will be at the end of the string if necessary.
     *
     *
     * std::string CentreJustify(const std::string p_Str, const std::size_t p_Width)
     *
     * @param   [IN]    p_Str                       String to be centre-justified
     * @param   [IN]    p_Width                     The required width of the resultant string
     * @return                                      String padded with leading and trailing spaces so as to (as close as possible) centre-justify p_Str
     *                                              The string returned will always be p_Width characters in length
     */
     std::string CentreJustify(const std::string p_Str, const std::size_t p_Width) {

        std::string result = p_Str;                                                                 // default is no change

        if (p_Str.length() < p_Width) {                                                             // p_Str < field width
            int numLeadingSpaces = (p_Width - p_Str.length()) / 2;                                  // number of spaces to add at start - half the deficit
            int numTralingSpaces = p_Width - p_Str.length() - numLeadingSpaces;                     // number of spaces to adda t the end - whatever is left
            std::string leadingSpaces(numLeadingSpaces, ' ');                                       // blank string to prepend
            std::string trailingSpaces(numTralingSpaces, ' ');                                      // blank string to append
            result = leadingSpaces + p_Str + trailingSpaces;                                        // add leading and trailing spaces to p_Str
        }

        return result;
    }


    /*
     * Formats value per sprintf() and returns string
     *
     * From https://stackoverflow.com/questions/2342162/stdstring-formatting-like-sprintf/49812018#49812018
     *
     *
     * string vFormat(const char* const p_zcFormat, ...)
     *
     * @param   [IN]    p_zcFormat                  Format string
     * @param   [IN]    ...                         Parameters to be formatted
     * @return                                      Formatted string
     */
    const std::string vFormat(const char* const p_zcFormat, ...) {

        // initialize use of the variable argument array
        va_list vaArgs;
        va_start(vaArgs, p_zcFormat);

        // reliably acquire the size from a copy of the variable argument array
        // and a functionally reliable call to mock the formatting
        va_list vaArgsCopy;
        va_copy(vaArgsCopy, vaArgs);
        const int iLen = std::vsnprintf(NULL, 0, p_zcFormat, vaArgsCopy);
        va_end(vaArgsCopy);

        // return a formatted string without risking memory mismanagement
        // and without assuming any compiler or platform specific behavior
        std::vector<char> zc(iLen + 1);
        std::vsnprintf(zc.data(), zc.size(), p_zcFormat, vaArgs);
        va_end(vaArgs);

        return std::string(zc.data(), iLen);
    }


    /*
     * Solve quadratic Ax^2 + Bx + C
     *
     * Returns either root, depending on discriminant will return:
     *
     *    0.0               if 0 roots
     *    root              if 1 root
     *    max(root1, root2) if 2 roots
     *
     *
     * double SolveQuadratic(const double p_A, const double p_B, double p_C)
     *
     * @param   [IN]    p_A                       Coefficient of x^2
     * @param   [IN]    p_B                       Coefficient of x^1
     * @param   [IN]    p_C                       Coefficient of x^0 (Constant)
     * @return                                    String padded with leading "0"s - will be unchanged from input string if length alread >= required length
     */
    double SolveQuadratic(const double p_A, const double p_B, double p_C) {

        double discriminant = (p_B * p_B) - (4.0 * p_A * p_C);      // d = B^2 - 4AC

        double root = 0.0;

        // JR: check < 0 first so don't have to check = 0.0 (will almost never happen after calculation - need epsilon)
        if (discriminant < 0.0) {                                   // no real roots (leave this as an absolute compare)
            std::cerr << "Error. No real roots" << std::endl;       // JR: todo: put proper warning in here
        }
        else if (discriminant > 0.0) {                              // 2 real roots (leae this as an absolute compare)
            double sqrtD = sqrt(discriminant);
            double A2    = p_A + p_A;

            double root1 = (-p_B + sqrtD) / A2;                     // (-B + SQRT(B^2 - 4AC)) / 2A
            double root2 = (-p_B - sqrtD) / A2;                     // (-B - SQRT(B^2 - 4AC)) / 2A

            root         = std::max(root1, root2);
        }
        else {                                                      // 1 real root
            root = -p_B / (p_A + p_A);                              // -B / 2A,discriminant = 0.0
        }

        return root;
    }


    /*
     * Sample from tabulated CDF
     *
     * Finds Y given X and an ordered map<double, double> of (X, Y) pairs
     * Uses simple linear interpolation
     *
     *
     * double SampleFromTabulatedCDF(const double p_X, const std::map<double, double> p_Table)
     *
     * @param   [IN]    p_X                 The X value for which Y is to be calculated
     * @param   [IN]    p_Table             The table to interpolate on (Ordered map<double, double> of (X, Y) pairs)
     * @return                              Interpolated Y value
     */
    double SampleFromTabulatedCDF(const double p_X, const std::map<double, double> pTable) {

        double yInterp = 0.0;

        std::map<double, double>::const_iterator iter;

        iter        = pTable.begin();
        double xMin = iter->first;

        iter        = pTable.end();
        double xMax = (--iter)->first;

        if (pTable.size() > 0 && (p_X >= xMin && p_X <= xMax)) {                                // sanity check - map must not be empty, and p_X must be in [xMin, xMax]  (Leave these as absolute compares)

            for (iter = pTable.begin(); (iter != pTable.end() && iter->first < p_X); ++iter);   // leave this as an absolute compare

            if (iter == pTable.begin()) {
                yInterp = iter->second;
            }
            else if (iter == pTable.end()) {
                yInterp = (--iter)->second;
            }
            else {

                double xAbove = iter->first;
            double yAbove = iter->second;
                    double xBelow = (--iter)->first;
                double yBelow = iter->second;

                double gradient = (yAbove - yBelow) / (xAbove - xBelow);

                yInterp = yBelow + ((p_X - xBelow) * gradient);
            }
        }

        return yInterp;
    }


    /*
     * Inverse sample from tabulated CDF
     *
     * Finds X given Y and an ordered map<double, double> of (X, Y) pairs
     * Uses simple linear interpolation
     *
     *
     * double InverseSampleFromTabulatedCDF(const double p_Y, const std::map<double, double> p_Table)
     *
     * @param   [IN]    p_Y                 The Y value for which X is to be calculated
     * @param   [IN]    p_Table             The table to interpolate on (Ordered map<double, double> of (X, Y) pairs)
     * @return                              Interpolated X value
     */
    double InverseSampleFromTabulatedCDF(const double p_Y, const std::map<double, double> p_Table) {

        double xInterp = 0.0;

        if (p_Table.size() > 0 && (p_Y >= 0.0 && p_Y < 1.0)) {                                  // sanity check - map must not be empty, and p_Y must in [0.0, 1.0)      (Levae these as absolute compares)

            std::map<double, double>::const_iterator iter;

            iter        = p_Table.begin();
            double yMin = iter->second;

            iter        = p_Table.end();
            double yMax = (--iter)->second;

            double y = yMin + (p_Y * (std::max(yMin, yMax) - std::min(yMax, yMin)));            // normalise y - clamp to [yMin, yMax)

            for (iter = p_Table.begin(); (iter != p_Table.end() && iter->second < y); ++iter);  // leave this as an absolute compare

            double xAbove = iter->first;
            double yAbove = iter->second;

            if (iter == p_Table.begin()) {
                xInterp = xAbove;
            }
            else {
                iter--;
                double xBelow = iter->first;
                double yBelow = iter->second;

                double gradient = (yAbove - yBelow) / (xAbove - xBelow);

                xInterp = xBelow + ((y - yBelow) / gradient);
            }
        }

        return xInterp;
    }


    /*
     * Draw sample from a power law distribution p(x) ~ x^(n) between p_Xmin and p_Xmax
     *
     * double InverseSampleFromPowerLaw(const double p_Power, const double p_Xmax, const double p_Xmin)
     *
     * @param   [IN]    p_Power             The power for the power law
     * @param   [IN]    p_Xmax              Maximum of the X-interval from which to sample
     * @param   [IN]    p_Xmin              Minimum of the X-interval from which to sample
     * @return                              Drawn sample
     */
    double InverseSampleFromPowerLaw(const double p_Power, const double p_Xmax, const double p_Xmin) {

        double result;

        double rand = RAND->Random();                   // Draw a random number between 0 and 1

        if (p_Power == -1.0) {                          // JR: todo: find a better way of doing this
            result = exp(rand * log(p_Xmax / p_Xmin)) * p_Xmin;
        }
        else {
            double powerPlus1     = p_Power + 1.0;
            double min_powerPlus1 = pow(p_Xmin, powerPlus1);

            result = pow((rand * (pow(p_Xmax, powerPlus1) - min_powerPlus1) + min_powerPlus1), 1.0 / powerPlus1);
        }

        return result;
    }


    /*
     * Convert a period in days to a semi-major axis in AU
     *
     *
     * double ConvertPeriodInDaysToSemiMajorAxisInAU(const double p_Mass1, const double p_Mass2, const double p_Period)
     *
     * @param   [IN]    p_Mass1             The mass of star 1 (Msol)
     * @param   [IN]    p_Mass2             The mass of star 2 (Msol)
     * @param   [IN]    p_Period            Orbital period (days)
     * @return                              Semi-major axis in AU
     */
    double ConvertPeriodInDaysToSemiMajorAxisInAU(const double p_Mass1, const double p_Mass2, const double p_Period) {

        double a_cubed_SI_top    = G * ((p_Mass1 * MSOL) + (p_Mass2 * MSOL)) * p_Period * p_Period * SECONDS_IN_DAY * SECONDS_IN_DAY;
        double a_cubed_SI_bottom = 4.0 * M_PI * M_PI;
        double a_cubed_SI        = a_cubed_SI_top / a_cubed_SI_bottom;
        double a_SI              = pow(a_cubed_SI, 1.0 / 3.0);

        return a_SI / AU;
    }


    /*
     * Calculate x^y where x is double and y is an integer
     *
     * faster than pow() for integer exponent
     *
     *
     * double intPow(const double p_Base, const int p_Exponent)
     *
     * @param   [IN]    p_Base              Base - number to be raised to integer power
     * @param   [IN]    p_Exponent          Exponent - integer to wich base should be raised
     * @return                              Base ^ Exponent
     */
    double intPow(const double p_Base, const int p_Exponent) {

        double result = 1.0;                                            // for exponent = 0

        if (p_Exponent != 0) {                                          // exponent not zero
            int times = abs(p_Exponent);                                // number of times to multiply
            for (int i = 0; i < times; i++) result *= p_Base;           // multiply
        }

        return p_Exponent < 0 ? 1.0 / result : result;                  // invert if negative exponent
    }


    /*
     * Determines if the stellar type passed is one of a list of stellar types passed
     *
     *
     * bool IsOneOf(const STELLAR_TYPE p_StellarType, const std::initializer_list<ST> p_List)
     *
     * @param   [IN]    p_StellarType               Stellar type to check
     * @param   [IN]    p_List                      List of stellar types
     * @return                                      Boolean - true if p_StellarType is in list, false if not
     */
    bool IsOneOf(const STELLAR_TYPE p_StellarType, const STELLAR_TYPE_LIST p_List) {
        for (auto elem: p_List)
            if (p_StellarType == elem) return true;
        return false;
    }


    /*
     * Returns a single SN type based on the SN_EVENT parameter passed
     * 
     * Returns (in priority order):
     *
     *    SN_EVENT::CCSN  iff CCSN  bit is set and USSN bit is not set
     *    SN_EVENT::ECSN  iff ECSN  bit is set
     *    SN_EVENT::PISN  iff PISN  bit is set
     *    SN_EVENT::PPISN iff PPISN bit is set
     *    SN_EVENT::USSN  iff USSN  bit is set
     *    SN_EVENT::NONE  otherwise
     * 
     *
     * @param   [IN]    p_SNEvent                   SN_EVENT mask to check for SN event type
     * @return                                      SN_EVENT
     */
    SN_EVENT SNEventType(const SN_EVENT p_SNEvent) {

        if ((p_SNEvent & (SN_EVENT::CCSN | SN_EVENT::USSN)) == SN_EVENT::CCSN ) return SN_EVENT::CCSN;
        if ((p_SNEvent & SN_EVENT::ECSN )                   == SN_EVENT::ECSN ) return SN_EVENT::ECSN;
        if ((p_SNEvent & SN_EVENT::PISN )                   == SN_EVENT::PISN ) return SN_EVENT::PISN;
        if ((p_SNEvent & SN_EVENT::PPISN)                   == SN_EVENT::PPISN) return SN_EVENT::PPISN;
        if ((p_SNEvent & SN_EVENT::USSN )                   == SN_EVENT::USSN ) return SN_EVENT::USSN;
        
        return SN_EVENT::NONE;
    }


    ///////////////////////////////////////////////////////////
    // 
    // Linear Algebra Functions
    // 
    ///////////////////////////////////////////////////////////

    /*
     * Calculate the standard dot product of 2 vectors
     *
     *
     * @param   [IN]   a                            first vector
     * @param   [IN]   b                            second vector
     * @return                                      dot product
     */
    double DotProduct(DBL_VECTOR a, DBL_VECTOR b) {
    
        double c = 0;
    
        for (int i=0; i<3; i++) {
            c += a[i]*b[i];
        }
    
        return c;
    }


    /*
     * Calculate the standard cross product of 2 vectors
     *
     *
     * @param   [IN]   a                            first vector
     * @param   [IN]   b                            second vector
     * @return                                      cross product
     */
    DBL_VECTOR CrossProduct(DBL_VECTOR a, DBL_VECTOR b) {
    
        DBL_VECTOR c{0, 0, 0};
    
        c[0] = a[1]*b[2] - a[2]*b[1];
        c[1] = a[2]*b[0] - a[0]*b[2];
        c[2] = a[0]*b[1] - a[1]*b[0];
    
        return c;


    /*
     * Calculate the magnitude of a velocity vector, the speed.
     * 
     *
     * @param   [IN]   velocity                      Velocity vector in 3D
     * @return                                      The associated speed
     */
    double CalculateSpeedFromVelocity(const DBL_VECTOR velocity) {
        // RTW 12/05/20 - rename this

        // Straightforward application of pythagorean theorem 
        double speed2 = velocity[0]*velocity[0] + velocity[1]*velocity[1] + velocity[2]*velocity[2]

        return sqrt(speed2)
    }


    /*
     * Rotates a vector from one coordinate basis to another using Euler Angles
     * 
     * Returns the rotated vector. For a change of basis from (X,Y,Z)->(X',Y',Z'),
     * ThetaE is the angle between Z and Z',
     * Vector N := Z x Z' (cross product)
     * PhiE is the angle between X and N
     * PsiE is the angle between X' and N
     * These angles uniquely determine the change of basis, which is applied 
     * in the form of a rotation matrix as a function of these angles.
     *
     * For details, see:
     * https://en.wikipedia.org/wiki/Euler_angles
     * https://en.wikipedia.org/wiki/Change_of_basis
     *
     *
     * @param   [IN]   p_oldVector                 The vector in old coordinate basis (any units, must be 3D)
     * @param   [IN]   p_ThetaE                    Euler angle Theta (rad) 
     * @param   [IN]   p_PhiE                      Euler angle Phi   (rad) 
     * @param   [IN]   p_PsiE                      Euler angle Psi   (rad) 
     * @return                                     rotatedVector
     */
    DBL_VECTOR RotateVector(const DBL_VECTOR p_oldVector, const double p_ThetaE, const double p_PhiE, const double p_PsiE) {

        // Replace for convenience, undefine below
        #define cTheta cos(p_ThetaE)
        #define cPhi   cos(p_PhiE)
        #define cPsi   cos(p_PsiE)
        #define sTheta sin(p_ThetaE)
        #define sPhi   sin(p_PhiE)
        #define sPsi   sin(p_PsiE)

        // Define the Rotation Matrix     
        std::vector<DBL_VECTOR> RotationMatrix = {
            { cPhi*cPsi - sPhi*cTheta*sPsi ,  -cPhi*sPsi - sPhi*cTheta*cPsi ,  sTheta*sPhi },
            { sPhi*cPsi + cPhi*cTheta*sPsi ,  -sPhi*sPsi + cPhi*cTheta*cPsi , -sTheta*cPhi },
            { sTheta*sPsi                  ,  sTheta*cPsi                   ,  cTheta      }};

        #undef cTheta
        #undef cPhi
        #undef cPsi
        #undef sTheta
        #undef sPhi
        #undef sPsi


        // Multiply RotationMatrix * p_oldVector
        DBL_VECTOR newVector = {0, 0, 0};

        for (int i=0; i< 3; i++) {
            for (int j=0; j<3; j++) {
                newVector[i] += RotationMatrix[i][j] * p_oldVector[j];
            }
        }

        return newVector;
    }

}
