#pragma once

class Fraction
{
private:
    int m_numerator{ 0 };
    int m_denominator{ 1 };

public:
    Fraction() { }

    Fraction(const int x, const int y)
        : m_numerator{ x }
        , m_denominator{ y }
    { }

    void print() const 
    {
        std::cout << m_numerator << "/" << m_denominator << '\n';
    }

    int getNumerator() const { return m_numerator; }
    int getDenominator() const { return m_denominator; }

    void setNumerator(const auto& x) { m_numerator = x; }
    void setDenominator(const auto& x) { m_denominator = x; }
};
