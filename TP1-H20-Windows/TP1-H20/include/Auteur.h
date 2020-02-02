#ifndef AUTEUR_H
#define AUTEUR_H

#include <string>

class Auteur
{
public:
    Auteur() = default;
    Auteur(const std::string& nom, unsigned int anneeDeNaissance);

    void afficher(std::ostream& stream) const;

    const std::string& getNom() const;
    unsigned int getAnneeDeNaissance() const;
    unsigned int getNbFilms() const;

    void setNbFilms(unsigned int nbFilms);
    friend std::ostream& operator<<(std::ostream& stream, const Auteur& auteur);
    bool operator==(const std::string& nom);
    friend bool operator==(const std::string& nom, const Auteur& auteur);

private:
    std::string nom_;
    unsigned int anneeDeNaissance_;
    unsigned int nbFilms_;
};

#endif // AUTEUR_H