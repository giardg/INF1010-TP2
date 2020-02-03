#include "Librairie.h"
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include "debogageMemoire.h"
#include "typesafe_enum.h"

namespace
{
    constexpr std::size_t CAPACITE_FILMS_INITIALE = 2;
    constexpr int FILM_INEXSISTANT = -1;
} // namespace

//! Constructeur de la classe Librairie
Librairie::Librairie()
//: films_(std::make_unique<Film[]>(CAPACITE_FILMS_INITIALE)) // pas sûr
//, nbFilms_(0)
//, capaciteFilms_(CAPACITE_FILMS_INITIALE)
{
}

Librairie::Librairie(const Librairie& l)
{
    films_.clear();
    for (int i = 0; i < l.films_.size(); i++)
    {
        auto film_ptr = std::make_unique<Film>(*(l.films_[i]));
        films_.push_back(std::move(film_ptr));
    }
}

Librairie& Librairie::operator=(const Librairie& l)
{
    if (this != &l)
    {
        films_.clear();
        for (int i = 0; i < (l.films_).size(); i++)
        {
            auto film_ptr = std::make_unique<Film>(*(l.films_[i]));
            films_.push_back(std::move(film_ptr));
        }
    }
    return *this;
}

//! Destructeur de la classe Librairie
Librairie::~Librairie()
{
    supprimerFilms();
}

const std::vector<std::unique_ptr<Film>>& Librairie::getFilms() const
{
    return films_;
}

//! Méthode qui ajoute un film à la liste des films
//! \param film Le film alloué dynamiquement à ajouter à la liste. La classe devient propriétaire du
//!             film.
// void Librairie::ajouterFilm(Film* film)
//{
//    static constexpr unsigned int AUGMENTATION_CAPACITE_FILMS = 2;
//
//    if (film == nullptr)
//    {
//        return;
//    }
//
//    // Verifier si assez de mémoire est allouée
//    if (nbFilms_ >= capaciteFilms_)
//    {
//        // Créer nouveau tableau
//        Film** nouvelleListe = new Film*[capaciteFilms_ * AUGMENTATION_CAPACITE_FILMS];
//
//        // Copier chaque élement vers le nouveau tableau
//        for (std::size_t i = 0; i < nbFilms_; i++)
//        {
//            nouvelleListe[i] = films_[i];
//        }
//
//        delete[] films_;
//        films_ = nouvelleListe; // Pointer vers la nouvelle adresse mémoire
//        capaciteFilms_ *= AUGMENTATION_CAPACITE_FILMS; // Mettre à jour la capacité
//    }
//
//    films_[nbFilms_++] = film;
//}

//! Méthode qui retire un film de la liste
//! \param nomFilm Le nom du film à retirer
// void Librairie::retirerFilm(const std::string& nomFilm)
//{
//    int indexFilm = trouverIndexFilm(nomFilm);
//    if (indexFilm == FILM_INEXSISTANT)
//    {
//        return;
//    }
//
//    delete films_[indexFilm];
//    films_[indexFilm] = films_[nbFilms_ - 1];
//    nbFilms_--;
//}

//! Méthode qui retourne un film comportant le même nom que celui envoyé en paramètre
//! \param nomFilm Le nom du film à chercher
//! \return        Un pointeur vers le film. Nullptr si le film n'existe pas
Film* Librairie::chercherFilm(const std::string& nomFilm)
{
    int indexFilm = trouverIndexFilm(nomFilm);
    if (indexFilm == FILM_INEXSISTANT)
    {
        return nullptr;
    }
    return films_[indexFilm].get();
}

//! Méthode qui charge les films à partir d'un fichier.
//! \param nomFichier           Le nom du fichier à lire.
//! \param gestionnaireAuteurs  Le gestionnaire des auteurs. Nécessaire pour associer un film à un
//! auteur.
//! \return                     Un bool représentant si le chargement a été un succès.
bool Librairie::chargerFilmsDepuisFichier(const std::string& nomFichier,
                                          GestionnaireAuteurs& gestionnaireAuteurs)
{
    std::ifstream fichier(nomFichier);
    if (fichier)
    {
        // Supprimers les vieux films avant de lire les nouveaux
        supprimerFilms();

        std::string ligne;
        while (std::getline(fichier, ligne))
        {
            if (lireLigneFilm(ligne, gestionnaireAuteurs) == false)
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

//! Méthode qui charge les restrictions des films à partir d'un fichier.
//! \param nomFichier           Le nom du fichier à lire.
//! \return                     Un bool représentant si le chargement a été un succès.
bool Librairie::chargerRestrictionsDepuisFichiers(const std::string& nomFichier)
{
    std::ifstream fichier(nomFichier);
    if (fichier)
    {
        for (std::size_t i = 0; i < films_.size(); i++)
        {
            films_[i]->supprimerPaysRestreints();
        }
        std::string ligne;
        while (std::getline(fichier, ligne))
        {
            if (lireLigneRestrictions(ligne) == false)
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

//! Méthode qui affiche la liste des films.
//! \param stream Le stream dans lequel afficher
// void Librairie::afficher(std::ostream& stream) const
//{
//    for (std::size_t i = 0; i < nbFilms_; i++)
//    {
//        films_[i]->afficher(stream);
//        stream << '\n';
//    }
//}

//! Méthode qui retourne le nombre de films
//! \return Le nombre de films
std::size_t Librairie::getNbFilms() const
{
    return films_.size();
}

Librairie& Librairie::operator+=(Film* film)
{
    if (film == nullptr)
    {
        return *this;
    }

    std::unique_ptr<Film> film_ptr;
    film_ptr.reset(film);
      

    // static constexpr unsigned int AUGMENTATION_CAPACITE_FILMS = 2;
    films_.push_back(std::move(film_ptr));
    return *this;
}

Librairie& Librairie::operator-=(const std::string& nomFilm)
{
    int indexFilm = trouverIndexFilm(nomFilm);
    if (indexFilm == FILM_INEXSISTANT)
    {
        return *this;
    }
    // films_[indexFilm].get()->getAuteur()->setNbFilms(
    //    films_[indexFilm].get()->getAuteur()->getNbFilms() - 1);
    films_[indexFilm] = std::move(films_[films_.size() - 1]);
    films_.pop_back();
    return *this;
}

//! Méthode qui supprime tous les films
void Librairie::supprimerFilms()
{
    films_.clear();
}

//! Méthode qui ajoute les restrictions d'un film avec un string
//! \param ligne Le string comportant les restrictions du film
//! \return      Un bool représentant si l'opération est un succès
bool Librairie::lireLigneRestrictions(const std::string& ligne)
{
    std::istringstream stream(ligne);
    std::string nomFilm;
    // Pour extraire tout ce qui se trouve entre "" dans un stream,
    // il faut faire stream >> std::quoted(variable)
    if (stream >> std::quoted(nomFilm))
    {
        Film* film = chercherFilm(nomFilm);
        if (film == nullptr)
        {
            // Film n'existe pas
            return false;
        }

        int paysValeurEnum;
        while (stream >> paysValeurEnum)
        {
            film->ajouterPaysRestreint(to_enum<Pays>(paysValeurEnum));
        }
        return true;
    }
    return false;
}

//! Méthode qui ajoute un film avec un string comportant les attributs du film
//! \param ligne                Le string comportant les attributs du film
//! \param gestionnaireAuteurs  Le gestionnaire des auteurs. Nécessaire pour associer un film à un
//!                             auteur.
//! \return                     Un bool représentant si l'opération est un succès
bool Librairie::lireLigneFilm(const std::string& ligne, GestionnaireAuteurs& gestionnaireAuteurs)
{
    std::istringstream stream(ligne);
    std::string nomFilm;
    unsigned int anneeSortie;
    int genreValeurEnum;
    int paysValeurEnum;
    bool estRestreintParAge;
    std::string nomAuteur;

    // Pour extraire tout ce qui se trouve entre "" dans un stream,
    // il faut faire stream >> std::quoted(variable)

    if (stream >> std::quoted(nomFilm) >> anneeSortie >> genreValeurEnum >> paysValeurEnum >>
        estRestreintParAge >> std::quoted(nomAuteur))
    {
        Auteur* auteur = gestionnaireAuteurs.chercherAuteur(nomAuteur);
        if (auteur == nullptr)
        {
            // Auteur non existant
            return false;
        }
        auteur->setNbFilms(auteur->getNbFilms() + 1);
        Film* film = new Film(nomFilm,
                              anneeSortie,
                              to_enum<Film::Genre>(genreValeurEnum),
                              to_enum<Pays>(paysValeurEnum),
                              estRestreintParAge,
                              auteur);

        *this += film;
        return true;
    }
    return false;
}

//! Méthode qui retourne l'index d'un film en cherchant son nom
//! \param nomFilm Le nom du film à trouver
//! \return        L'index du film. Retourne -1 si le film n'existe pas
int Librairie::trouverIndexFilm(const std::string& nomFilm) const
{
    for (std::size_t i = 0; i < films_.size(); i++)
    {
        if (films_[i]->getNom() == nomFilm)
        {
            return (int)i;
        }
    }
    return FILM_INEXSISTANT;
}

std::ostream& operator<<(std::ostream& stream, const Librairie& librairie)
{
    for (std::size_t i = 0; i < librairie.films_.size(); i++)
    {
        stream << *(librairie.films_[i]);
        stream << '\n';
    }
    return stream;
}
