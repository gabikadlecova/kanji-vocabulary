#ifndef KANJI_DATA_H
#define KANJI_DATA_H

#include <vector>
#include <istream>

#include <chrono>

//! namespace for manipulation with kanji data
namespace kanji_data
{
    using time = std::chrono::system_clock::time_point; /*!< Time of the last repetition*/

    /*!
     * \brief The kanji_compound class is a word represented by kanji characters.
     * This class represents a single word from a dictionary composed by kanji
     * and/or hiragana, along with its reading and meaning (translation).
     * It also contains a unique id, time of the last repetition and level.
     */
	class kanji_compound {
	public:
		using kanji_id = std::uint_least32_t;
		using kanji_level = std::int_least32_t;

        /*!
         * \brief kanji_compound Construct an empty kanji compount
         * The compound id and level is invalid, which means that the
         * compound is not present in the library.
         */
        kanji_compound() :
            reading{L""},
            meaning{L""},
            id_valid_(false),
            id_(0),
            kanji_str_{ L"" },
            level_{ 0 }
        {
            last_rep_ = std::chrono::system_clock::now();
        }

        /*!
         * \brief kanji_compound Construct a kanji compound with all data provided.
         * \param k_str Kanji characters of the compound.
         * \param reading Reading of the kanji.
         * \param meaning Meaning of the compound (translation).
         * \param id Unique id.
         * \param level Repetition level.
         * \param last_rep Time of the last repetition.
         */
        kanji_compound(std::wstring k_str, std::wstring reading, std::wstring meaning,
                       const kanji_id id, const kanji_level level, time last_rep) :
            reading{ std::move(reading) },
            meaning{ std::move(meaning) },
            id_valid_(true),
            id_(id),
            kanji_str_{ std::move(k_str) },
            last_rep_{ last_rep },
            level_{ level }
        {}

        /*!
         * \brief kanji_compound Construct a kanji compound only with the lexical data.
         * \param k_str Kanji characters of the compound.
         * \param reading Reading of the kanji.
         * \param meaning Meaning of the compound (translation).
         * This constructor should be used if a new kanji compound needs to be created,
         * i.e. on addition.
         */
        kanji_compound(std::wstring k_str, std::wstring reading, std::wstring meaning) :
            reading{ std::move(reading) },
            meaning{ std::move(meaning) },
            id_valid_(false),
            id_(0),
            kanji_str_{ std::move(k_str) },
            level_{ 1 }
		{
			last_rep_ = std::chrono::system_clock::now();
		}

        /*!
         * \brief repeat Changes level according to \p succeeded
         * \param succeeded Determines whether the repetition (training) was successfull.
         * If the training succeeds, the level is doubled, else it is halved.
         * The level does not go below 1 and does not exceed #max_level
         */
		void repeat(bool succeeded);
        /*!
         * \brief reset_time Sets the time of the last repetition to "now".
         */
		void reset_time();

        std::wstring reading; /*!< Reading of the kanji compound. */
        std::wstring meaning; /*!< Translation of the compound.*/

        /*! Returns the unique identifier. */
		kanji_id get_id() const { return id_; }
        /*! Determines whether get_id() returns a valid identifier. */
		bool id_valid() const { return id_valid_; }

        /*! Returns kanji of the compound. */
		const std::wstring &get_kanji() const { return kanji_str_; }

        /*! Returns the time of the last repetition. */
        time get_last_rep() const { return last_rep_; }
        /*! Returns kanji level (between 1 and #max_level ) */
		kanji_level get_level() const { return level_; }
	private:
        bool id_valid_;

		kanji_id id_;
		std::wstring kanji_str_;

        static const kanji_level max_level = 32; /*!< Maximum level of the compound. */
		time last_rep_;
		kanji_level level_;
	};

    /*!
     * \brief The kanji_lib class Represents a collection of unique kanji compounds.
     * The kanji library provides CRUD operations for kanji compounds. All compounds
     * can be uniquely identified by a valid id.
     */
	class kanji_lib {
	public:
        /*!
         * \brief kanji_lib Construct a new library of kanji compounds.
         * \param kanji Vector of kanji compounds.
         * \param id Value greater than all kanji ids.
         * The \p kanji vector should contain only kanjis with valid ids, otherwise
         * undefined behaviour may occur.
         * Similarly \p id should be greater than or equal to the maximum of all
         * compound ids, as otherwise there could be generated compounds with
         * a duplicate id, which also results in undefined behaviour.
         * It does not have to be exactly the maximum though.
         */
        kanji_lib(std::vector<kanji_compound> kanji, const kanji_compound::kanji_id id) :
            max_id_(id),
            kanji_(std::move(kanji))
        {}

        /*!
         * \brief get_kanji Returns a reference kanji compounds from the library.
         * \return Reference to the kanji vector.
         */
        std::vector<kanji_compound> &get_kanji() { return kanji_; }
        /*!
         * \brief get_kanji Returns a constant reference kanji compounds from the library.
         * \return Constant reference to the kanji vector.
         */
        const std::vector<kanji_compound> &get_kanji() const { return kanji_; }

        /*!
         * \brief add_kanji Adds a new kanji compound to the library.
         * \param kanji_str Kanji characters of the compound.
         * \param reading Reading of the kanji.
         * \param meaning Translation of the compound.
         * \return Returns the added kanji with valid id, level and time.
         * Adds a new kanji compound to the library and sets its library
         * and training specific members (id, level,...).
         * Throws std::logic_error if a compound with the same \p kanji_str
         * exists in the library.
         */
        kanji_compound add_kanji(std::wstring kanji_str,
                                 std::wstring reading, std::wstring meaning);
        /*!
         * \brief update_kanji Updates a kanji compound in the library.
         * \param kanji Kanji compound with a library-valid id.
         * Updates a kanji compound. It must be already present in the
         * library, which means that there must be a compound with the
         * same id.
         */
		void update_kanji(kanji_compound kanji);
        /*!
         * \brief delete_kanji Deletes a compound with the given id from the library.
         * \param id Id of the kanji to be deleted.
         * Deletes a compound from the library, if it has the same id as the parameter.
         */
        void delete_kanji(kanji_compound::kanji_id id);
        /*!
         * \brief reset_time Sets repetition time of all compounds to "now".
         */
		void reset_time();
	private:
		kanji_compound::kanji_id max_id_;
		std::vector<kanji_compound> kanji_;
	};

    /*!
     * \brief The duplicate_error class Exception object, is thrown if duplicates occur.
     * This exception is thrown if the user tries to add a kanji compound whose
     * kanji character string is already present in the library.
     */
    class duplicate_error : public std::logic_error {

    public:
        duplicate_error(const std::string &what_arg) :
            std::logic_error(what_arg) {}
        duplicate_error(const char *what_arg) :
            std::logic_error(what_arg) {}
        virtual ~duplicate_error() = default;
    };
    /*!
     * \brief The format_error class Exception object, is thrown if the input is bad.
     * This exception is thrown if the provided input file has an invalid format.
     */
    class format_error: public std::logic_error {

    public:
        format_error(const std::string &what_arg) :
            std::logic_error(what_arg) {}
        format_error(const char *what_arg) :
            std::logic_error(what_arg) {}

        virtual ~format_error() = default;
    };

    /*!
     * \brief The format_error class Exception object, is thrown if a kanji is missing.
     * This exception is thrown if a kanji compound referenced by an identifier
     * is missing in the library, but is handled as if it were there (e.g. update).
     */
    class missing_error : public std::logic_error {

    public:
        missing_error(const std::string &what_arg) :
            std::logic_error(what_arg) {}
        missing_error(const char *what_arg) :
            std::logic_error(what_arg) {}
        virtual ~missing_error() = default;
    };

    using const_kanji_it = std::vector<kanji_compound>::const_iterator;

    /*!
     * \brief by_kanji Filters the library according to kanji characters.
     * \param lib Library to be searched.
     * \param k Kanji character filter.
     * \return Vector of kanji compounds which contain \p k.
     * Searches the library for all compounds, whose kanji string contains \p k as
     * its substring.
     */
    const_kanji_it by_kanji(const_kanji_it begin, const_kanji_it end,
                            const std::wstring &k);
    /*!
     * \brief by_kanji Filters the library according to meaning of a compound.
     * \param lib Library to be searched.
     * \param m Meaning filter.
     * \return Vector of kanji compounds whose meanings contain \p m.
     * Searches the library for all compounds, whose meaning contains \p m as
     * its substring.
     */
    const_kanji_it by_meaning(const_kanji_it begin, const_kanji_it end,
                              const std::wstring &m);
    /*!
     * \brief by_kanji Filters the library according to the reading of a compound.
     * \param lib Library to be searched.
     * \param r Reading filter.
     * \return Vector of kanji compounds whose readings contain \p r.
     * Searches the library for all compounds, whose reading contains \p r as
     * its substring.
     */
    const_kanji_it by_reading(const_kanji_it begin, const_kanji_it end,
                              const std::wstring &r);
    /*!
     * \brief due_today Searches the library for all compounds which are to be repeated.
     * \param lib Library to be searched.
     * \return Vector of compounds which should be repeated now.
     * Searches the library for all kanji, which were last repeated before
     * kanji_compound::level_ days.
     */
    const_kanji_it due_today(const_kanji_it begin, const_kanji_it end);

    /*!
     * \brief empty_lib Creates a library with no kanji compounds added.
     * \return An empty library.
     */
    kanji_lib empty_lib();
    /*!
     * \brief read_lib Reads a kanji compound library from the input stream of wchars.
     * \param is Input wchar stream.
     * \return A library of kanji compounds.
     */
	kanji_lib read_lib(std::wistream &is);
    /*!
     * \brief write_lib Writes a library to the output wchar stream.
     * \param lib Library to be written.
     * \param wos Output wchar stream.
     */
	void write_lib(const kanji_lib &lib, std::wostream &wos);
}

#endif // !KANJI_DATA_H
