// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef PROPERTIES_HPP_
#define PROPERTIES_HPP_

// *****************************************************************************
#include "exiv2lib_export.h"

// included header files
#include <mutex>

#include "datasets.hpp"

// *****************************************************************************
// namespace extensions
namespace Exiv2 {
// *****************************************************************************
// class declarations
class XmpKey;

// *****************************************************************************
// class definitions

//! Category of an XMP property
enum XmpCategory { xmpInternal, xmpExternal };

//! Information about one XMP property.
struct EXIV2API XmpPropertyInfo {
  //! Comparison operator for name
  bool operator==(const std::string& name) const;

  const char* name_;          //!< Property name
  const char* title_;         //!< Property title or label
  const char* xmpValueType_;  //!< XMP value type (for info only)
  TypeId typeId_;             //!< Exiv2 default type for the property
  XmpCategory xmpCategory_;   //!< Category (internal or external)
  const char* desc_;          //!< Property description
};

//! Structure mapping XMP namespaces and (preferred) prefixes.
struct EXIV2API XmpNsInfo {
  //! For comparison with prefix
  struct Prefix {
    //! The prefix string.
    std::string prefix_;
  };
  //! For comparison with namespace
  struct Ns {
    //! The namespace string
    std::string ns_;
  };
  //! Comparison operator for namespace
  bool operator==(const Ns& ns) const;
  //! Comparison operator for prefix
  bool operator==(const Prefix& prefix) const;

  const char* ns_;                          //!< Namespace
  const char* prefix_;                      //!< (Preferred) prefix
  const XmpPropertyInfo* xmpPropertyInfo_;  //!< List of known properties
  const char* desc_;                        //!< Brief description of the namespace
};

//! XMP property reference, implemented as a static class.
class EXIV2API XmpProperties {
 private:
  static const XmpNsInfo* nsInfoUnsafe(const std::string& prefix);
  static void unregisterNsUnsafe(const std::string& ns);
  static const XmpNsInfo* lookupNsRegistryUnsafe(const XmpNsInfo::Prefix& prefix);

 public:
  /*!
    @brief Return the title (label) of the property.
    @param key The property key
    @return The title (label) of the property, 0 if the
           key is of an unknown property.
   */
  static const char* propertyTitle(const XmpKey& key);
  /*!
    @brief Return the description of the property.
    @param key The property key
    @return The description of the property, 0 if the
           key is of an unknown property.
   */
  static const char* propertyDesc(const XmpKey& key);
  /*!
    @brief Return the type for property \em key. The default
           for unknown keys is xmpText.
    @param key The property key
    @return The type of the property
   */
  static TypeId propertyType(const XmpKey& key);
  /*!
    @brief Return information for the property for key.

    If the key is a path to a nested property (one which contains a slash,
    like \c Xmp.MP.RegionInfo/MPRI:Regions), determines the innermost element
    (\c Xmp.MPRI.Regions) and returns its property information.

    @param key The property key
    @return A pointer to the property information, 0 if the
           key is of an unknown property.
   */
  static const XmpPropertyInfo* propertyInfo(const XmpKey& key);
  /*!
     @brief Return the namespace name for the schema associated
            with \em prefix.
     @param prefix Prefix
     @return The namespace name
     @throw Error if no namespace is registered with \em prefix.
   */
  static std::string ns(const std::string& prefix);
  /*!
     @brief Return the namespace description for the schema associated
            with \em prefix.
     @param prefix Prefix
     @return The namespace description
     @throw Error if no namespace is registered with \em prefix.
   */
  static const char* nsDesc(const std::string& prefix);
  /*!
    @brief Return read-only list of built-in properties for \em prefix.
    @param prefix Prefix
    @return Pointer to the built-in properties for prefix, may be 0 if
            none is configured in the namespace info.
    @throw Error if no namespace is registered with \em prefix.
   */
  static const XmpPropertyInfo* propertyList(const std::string& prefix);
  /*!
    @brief Return information about a schema namespace for \em prefix.
           Always returns a valid pointer.
    @param prefix The prefix
    @return A pointer to the related information
    @throw Error if no namespace is registered with \em prefix.
   */
  static const XmpNsInfo* nsInfo(const std::string& prefix);

  /*!
     @brief Return the (preferred) prefix for schema namespace \em ns.
     @param ns Schema namespace
     @return The prefix or an empty string if namespace \em ns is not
            registered.
   */
  static std::string prefix(const std::string& ns);
  //! Print a list of properties of a schema namespace to output stream \em os.
  static void printProperties(std::ostream& os, const std::string& prefix);

  //! Interpret and print the value of an XMP property
  static std::ostream& printProperty(std::ostream& os, const std::string& key, const Value& value);
  /*!
    @brief Register namespace \em ns with preferred prefix \em prefix.

    If the prefix is a known or previously registered prefix, the
    corresponding namespace URI is overwritten.

    @note This invalidates XMP keys generated with the previous prefix.
   */
  static void registerNs(const std::string& ns, const std::string& prefix);
  /*!
    @brief Unregister a custom namespace \em ns.

    The function only has an effect if there is a namespace \em ns
    registered earlier, it does not unregister built-in namespaces.

    @note This invalidates XMP keys generated in this namespace.
   */
  static void unregisterNs(const std::string& ns);

  /*!
    @brief Lock to be used while modifying properties.

    @todo For a proper read-write lock, this shall be improved by a
    \em std::shared_timed_mutex (once C++14 is allowed) or
    \em std::shared_mutex (once C++17 is allowed). The
    read-access locks shall be updated to \em std::shared_lock then.
   */
  static std::mutex mutex_;

  /*!
    @brief Unregister all custom namespaces.

    The function only unregisters namespaces registered earlier, it does not
    unregister built-in namespaces.

    @note This invalidates XMP keys generated in any custom namespace.
   */
  static void unregisterNs();
  //! Type for the namespace registry
  using NsRegistry = std::map<std::string, XmpNsInfo>;
  /*!
    @brief Get the registered namespace for a specific \em prefix from the registry.
   */
  static const XmpNsInfo* lookupNsRegistry(const XmpNsInfo::Prefix& prefix);

  // DATA
  static NsRegistry nsRegistry_;  //!< Namespace registry

  /*!
    @brief Get all registered namespaces (for both Exiv2 and XMPsdk)
   */
  static void registeredNamespaces(Exiv2::Dictionary& nsDict);

};  // class XmpProperties

/*!
  @brief Concrete keys for XMP metadata.
 */
class EXIV2API XmpKey : public Key {
 public:
  //! Shortcut for an %XmpKey auto pointer.
  using UniquePtr = std::unique_ptr<XmpKey>;

  //! @name Creators
  //@{
  /*!
    @brief Constructor to create an XMP key from a key string.

    @param key The key string.
    @throw Error if the first part of the key is not '<b>Xmp</b>' or
           the second part of the key cannot be parsed and converted
           to a known (i.e., built-in or registered) schema prefix.
  */
  explicit XmpKey(const std::string& key);
  /*!
    @brief Constructor to create an XMP key from a schema prefix
           and a property name.

    @param prefix   Schema prefix name
    @param property Property name

    @throw Error if the schema prefix is not known.
  */
  XmpKey(const std::string& prefix, const std::string& property);
  //! Copy constructor.
  XmpKey(const XmpKey& rhs);
  //! Virtual destructor.
  ~XmpKey() override;
  //@}

  //! @name Manipulators
  //@{
  //! Assignment operator.
  XmpKey& operator=(const XmpKey& rhs);
  //@}

  //! @name Accessors
  //@{
  [[nodiscard]] std::string key() const override;
  [[nodiscard]] const char* familyName() const override;
  /*!
    @brief Return the name of the group (the second part of the key).
           For XMP keys, the group name is the schema prefix name.
  */
  [[nodiscard]] std::string groupName() const override;
  [[nodiscard]] std::string tagName() const override;
  [[nodiscard]] std::string tagLabel() const override;
  [[nodiscard]] std::string tagDesc() const override;
  //! Properties don't have a tag number. Return 0.
  [[nodiscard]] uint16_t tag() const override;

  [[nodiscard]] UniquePtr clone() const;

  // Todo: Should this be removed? What about tagLabel then?
  //! Return the schema namespace for the prefix of the key
  [[nodiscard]] std::string ns() const;
  //@}

 private:
  //! Internal virtual copy constructor.
  [[nodiscard]] XmpKey* clone_() const override;

  // Pimpl idiom
  struct Impl;
  std::unique_ptr<Impl> p_;

};  // class XmpKey

// *****************************************************************************
// free functions

//! Output operator for property info
EXIV2API std::ostream& operator<<(std::ostream& os, const XmpPropertyInfo& propertyInfo);

}  // namespace Exiv2

#endif  // #ifndef PROPERTIES_HPP_
