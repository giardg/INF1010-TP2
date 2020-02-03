#include "GestionnaireAuteurs.h"
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

//! Constructeur de la classe GestionnaireAuteurs
GestionnaireAuteurs::GestionnaireAuteurs()
    : auteurs_(std::vector<Auteur>(NB_AUTEURS_MAX)) // ne pas changer. le vecteur doit etre initialiser a 16 ici
{
}

//! Méthode qui ajoute un auteur à la liste des auteurs
//! \param auteur L'auteur à ajouter
//! \return       Un bool représentant si l'opération a fonctionnné
//bool GestionnaireAuteurs::ajouterAuteur(const Auteur& auteur)
//{
//    if (nbAuteurs_ >= NB_AUTEURS_MAX)
//    {
//        return false;
//    }
//
//    auteurs_[nbAuteurs_++] = auteur;
//    return true;
//}

//! Méhode qui cherche un auteur par son nom complet
//! \param nomAuteur Le nom de l'auteur à trouver
//! \return          Un pointeur vers l'auteur. Le pointeur est nullptr si l'auteur n'existe pas.
Auteur* GestionnaireAuteurs::chercherAuteur(const std::string& nomAuteur)
{
    for (std::size_t i = 0; i < auteurs_.size(); i++)
    {
        if (auteurs_[i] == nomAuteur)
        {
            return &auteurs_[i];
        }
    }

    return nullptr;
}

//! Méthode qui prend un nom de fichier en paramètre et qui charge les auteurs
//! \param nomFichier   Le nom du fichier à charger
//! \return             Un bool représentant si le chargement a été un succès
bool GestionnaireAuteurs::chargerDepuisFichier(const std::string& nomFichier)
{
    std::ifstream fichier(nomFichier);
    if (fichier)
    {
        auteurs_.clear();
        std::string ligne;
        while (std::getline(fichier, ligne))
        {
            if (lireLigneAuteur(ligne) == false)
            {
                return false;
            }
        }
        return true;
    }
    std::cerr << "Le fichier " << nomFichier
              << " n'existe pas. Assurez vous de le mettre au bon endroit.\n";
    return false;
}

//! Méthode qui affiche la liste des auteurs
//! \param stream Le stream dans lequel afficher
//void GestionnaireAuteurs::afficher(std::ostream& stream) const
//{
//    for (std::size_t i = 0; i < nbAuteurs_; i++)
//    {
//        auteurs_[i].afficher(stream);
//        stream << '\n';
//    }
//}

//! Méthode qui retourne le nombre d'auteurs
//! \return Le nombre d'auteurs
std::size_t GestionnaireAuteurs::getNbAuteurs() const
{
    return auteurs_.size();
}

bool GestionnaireAuteurs::operator+=(const Auteur& auteur)
{
    if (auteurs_.size() >= NB_AUTEURS_MAX)
    {
        return false;
    }
    
    auteurs_.push_back(auteur);
    return true;
}

//! Méthode qui ajoute un auteur avec un string
//! \param ligne Le string qui comporte tous les attributs de l'auteur
bool GestionnaireAuteurs::lireLigneAuteur(const std::string& ligne)
{
    std::istringstream stream(ligne);
    std::string nomAuteur;
    unsigned int age;

    // Pour extraire tout ce qui se trouve entre "" dans un stream,
    // il faut faire stream >> std::quoted(variable)

    if (stream >> std::quoted(nomAuteur) >> age)
    {
        bool succes = *this += Auteur(nomAuteur, age); // poh sûr
        return succes;
    }
    return false;
}

std::ostream& operator<<(std::ostream& stream, const GestionnaireAuteurs& auteurs)
{
    for (std::size_t i = 0; i < auteurs.auteurs_.size(); i++)
    {
        stream << auteurs.auteurs_[i];
        stream << '\n';
    }
    return stream;
}