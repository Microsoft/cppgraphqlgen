// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#ifndef REQUESTLOADER_H
#define REQUESTLOADER_H

#include "GeneratorLoader.h"

#include "graphqlservice/GraphQLGrammar.h"
#include "graphqlservice/GraphQLParse.h"
#include "graphqlservice/GraphQLSchema.h"
#include "graphqlservice/GraphQLService.h"

namespace graphql::generator {

struct ResponseField;

using ResponseFieldList = std::vector<ResponseField>;

struct ResponseType
{
	std::shared_ptr<const schema::BaseType> type;
	std::string_view cppType;
	ResponseFieldList fields;
};

using ResponseUnionOptions = std::vector<ResponseType>;
using ResponseFieldChildren = std::variant<ResponseFieldList, ResponseUnionOptions>;

struct ResponseField
{
	std::shared_ptr<const schema::BaseType> type;
	std::string_view name;
	std::string_view cppName;
	std::string_view defaultValueString;
	response::Value defaultValue;
	std::optional<tao::graphqlpeg::position> position;
	std::optional<ResponseFieldChildren> children;
};

struct RequestOptions
{
	const std::string requestFilename;
	const std::string operationName;
	const bool noIntrospection = false;
};

class SchemaLoader;

class RequestLoader
{
public:
	explicit RequestLoader(RequestOptions&& requestOptions, const SchemaLoader& schemaLoader);

	std::string_view getRequestFilename() const noexcept;
	std::string_view getOperationDisplayName() const noexcept;
	std::string getOperationNamespace() const noexcept;
	std::string_view getOperationType() const noexcept;
	std::string_view getRequestText() const noexcept;

	const ResponseFieldList& getVariables() const noexcept;
	const ResponseType& getResponseType() const noexcept;

private:
	void buildSchema();
	void addTypesToSchema();
	std::shared_ptr<const schema::BaseType> getSchemaType(
		std::string_view type, const TypeModifierStack& modifiers) const noexcept;
	void validateRequest() const;

	static std::string_view trimWhitespace(std::string_view content) noexcept;

	void findOperation();
	void collectVariables() noexcept;
	void collectFragments() noexcept;

	using FragmentDefinitionMap = std::map<std::string_view, const peg::ast_node*>;

	// SelectionVisitor visits the AST and fills in the ResponseType for the request.
	class SelectionVisitor
	{
	public:
		explicit SelectionVisitor(const FragmentDefinitionMap& fragments,
			const std::shared_ptr<schema::Schema>& schema,
			const std::shared_ptr<const schema::BaseType>& type);

		void visit(const peg::ast_node& selection);

		ResponseFieldList getFields();

	private:
		void visitField(const peg::ast_node& field);
		void visitFragmentSpread(const peg::ast_node& fragmentSpread);
		void visitInlineFragment(const peg::ast_node& inlineFragment);

		const std::optional<std::reference_wrapper<const service::field_path>> _path;
		const FragmentDefinitionMap& _fragments;
		const std::shared_ptr<schema::Schema>& _schema;
		const std::shared_ptr<const schema::BaseType>& _type;

		internal::string_view_set _names;
		ResponseFieldList _fields;
	};

	const RequestOptions _requestOptions;
	const SchemaLoader& _schemaLoader;
	std::shared_ptr<schema::Schema> _schema;
	peg::ast _ast;

	std::string _requestText;
	const peg::ast_node* _operation = nullptr;
	std::string_view _operationName;
	std::string_view _operationType;
	ResponseFieldList _variables;
	ResponseType _responseType;

	FragmentDefinitionMap _fragments;
};

} /* namespace graphql::generator */

#endif // REQUESTLOADER_H