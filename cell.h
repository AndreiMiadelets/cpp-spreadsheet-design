#pragma once

#include "common.h"
#include "formula.h"
#include "sheet.h"

#include <unordered_set>

class Cell : public CellInterface {
public:
  using UniquePositions = std::unordered_set<Position>;

  Cell();
  ~Cell() override = default;

  void Set(std::string text) override;
  void Clear();

  Value GetValue() const override;
  std::string GetText() const override;

  std::unordered_set<Position> GetParentCells() const;
  std::unordered_set<Position> GetChildrenCells() const;
  bool IsReferenced() const;
  void ClearCache();

private:
  class Impl;
  class EmptyImpl;
  class TextImpl;
  class FormulaImpl;

  std::unique_ptr<Impl> CreateFormulaImpl(std::string text) const;
  void SetImpl(std::string text, std::unique_ptr<Impl>&& formula_impl);

  std::unique_ptr<Impl> impl_;
  std::unordered_set<Cell *> parents_;   // Ячейки, от которых зависит текущая
  std::unordered_set<Cell *> children_;  // Ячейки, которые зависят от текущей
};

class Cell::Impl {
public:
  virtual ~Impl() = default;

  virtual Value GetValue() const = 0;
  virtual std::string GetText() const = 0;
};

class Cell::EmptyImpl : public Cell::Impl {
public:
  Value GetValue() const override;
  std::string GetText() const override;

private:
  std::string empty_text_;
};

class Cell::TextImpl : public Cell::Impl {
public:
  explicit TextImpl(std::string text);

  Value GetValue() const override;
  std::string GetText() const override;

private:
  std::string text_;
};

class Cell::FormulaImpl : public Cell::Impl {
public:
  explicit FormulaImpl(const std::string& text);

  Value GetValue() const override;
  std::string GetText() const override;

private:
  std::unique_ptr<FormulaInterface> formula_;
};
