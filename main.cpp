#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>

namespace figure {

struct TPoint {
    int32_t x = 0;
    int32_t y = 0;

    TPoint() = default;
    TPoint(const int32_t pX, const int32_t pY) : x(pX), y(pY) {}
};

enum class ELineType : uint32_t {
    SOLID,
    DASHED,
    DOTTED
};

std::string GetLineTypeStr(const ELineType lineType) {
    switch(lineType) {
        case ELineType::SOLID: return "Solid";
        case ELineType::DASHED: return "Dashed";
        case ELineType::DOTTED: return "Dotted";
    }

    return "ERROR";
}

enum class ELineThickness : uint32_t {
    THIN,
    NORMAL,
    THICK
};

std::string GetLineThicknessStr(const ELineThickness lineThikness) {
    switch(lineThikness) {
        case ELineThickness::THIN: return "Thin";
        case ELineThickness::NORMAL: return "Normal";
        case ELineThickness::THICK: return "Thick";
    }

    return "ERROR";
}

class TFigure {
protected:
    uint32_t id_;
    TPoint point_;
    ELineType lineType_ = ELineType::SOLID;
    ELineThickness lineThickness_ = ELineThickness::NORMAL;

    TFigure() = default;

public:
    virtual ~TFigure() { std::cout << "Delete TFigure" << std::endl; }

    virtual void Print() const {
        std::cout << "Figure id: " << id_ << separator
                  << "x: " + std::to_string(point_.x) << separator
                  << "y: " + std::to_string(point_.y) << separator
                  << "Line type: " << GetLineTypeStr(lineType_) << separator
                  << "Line thickness: " << GetLineThicknessStr(lineThickness_);
    }

    void SetLineType(const ELineType lineType) { lineType_ = lineType; }
    void SetLineThickness(const ELineThickness lineThickness) { lineThickness_ = lineThickness; }

private:
    const std::string separator = "; ";
};

class TCircle final : public TFigure {
public:
    TCircle() = default;
    TCircle(const uint32_t figureID, TPoint&& p) { point_ = std::move(p); id_ = figureID; }
    virtual ~TCircle() { std::cout << "Delete TCircle" << std::endl; }

    virtual void Print() const override {
        std::cout << "Circle: [";
        static_cast<const TFigure*>(this)->TFigure::Print();
        std::cout << "]" << std:: endl;
    }
};

class TSquare final : public TFigure {
public:
    TSquare() = default;
    TSquare(const uint32_t figureID, TPoint&& p) { point_ = std::move(p); id_ = figureID; }
    virtual ~TSquare() { std::cout << "Delete TSquare" << std::endl;}

    virtual void Print() const override {
        std::cout << "Square: [";
        static_cast<const TFigure*>(this)->TFigure::Print();
        std::cout << "]" << std:: endl;
    }
};

class TTriangle final : public TFigure {
public:
    TTriangle() = default;
    TTriangle(const uint32_t figureID, TPoint&& p) { point_ = std::move(p); id_ = figureID; }
    virtual ~TTriangle() { std::cout << "Delete TTriangle" << std::endl; }

    virtual void Print() const override {
        std::cout << "Triangle: [";
        static_cast<const TFigure*>(this)->TFigure::Print();
        std::cout << "]" << std:: endl;
    }
};

class TStar final : public TFigure {
public:
    TStar() = delete;
    TStar(const uint32_t figureID, TPoint&& p) { point_ = std::move(p); id_ = figureID; }
    virtual ~TStar() { std::cout << "Delete TStar" << std::endl; }

    virtual void Print() const override {
        std::cout << "Star: [";
        static_cast<const TFigure*>(this)->TFigure::Print();
        std::cout << "]" << std:: endl;
    }
};

class TSmartFigure {
    TFigure* ptr_ = nullptr;

public:
    TSmartFigure() = default;
    TSmartFigure(TFigure* const f) : ptr_(f) {}
    ~TSmartFigure() { delete ptr_; }

    TSmartFigure& Print() {
        ptr_->Print();
        return *this;
    }

    TSmartFigure& SetLineType(const ELineType lineType) {
        ptr_->SetLineType(lineType);
        return *this;
    }

    TSmartFigure& SetLineThickness(const ELineThickness lineThickness) {
        ptr_->SetLineThickness(lineThickness);
        return *this;
    }
};

} // namespace figure

// TODO вынести эти функции в отдельный класс CommandReader
figure::TPoint GetCoordinates() {
    int32_t x = 0, y = 0;
    std::cout << "Enter x and y coordinates:\n"
              << "x: ";
    std::cin >> x;
    std::cout << "y: ";
    std::cin >> y;

    return { x, y };
}

uint32_t GetFigureID() {
    uint32_t id = 0;
    std::cout << "Enter figure id: ";
    std::cin >> id;

    return id;
}

figure::ELineType GetLineType() {
    uint32_t lineType = 0;
    std::cout << "Enter line type: \n"
              << "\t1. Solid\n"
              << "\t2. Dashed\n"
              << "\t3. Dotted\n";
    std::cin >> lineType;
    return static_cast<figure::ELineType>(lineType - 1);
}

figure::ELineThickness GetLineThickness() {
    uint32_t lineThickness = 0;
    std::cout << "Enter line thickness: \n"
              << "\t1. Thin\n"
              << "\t2. Normal\n"
              << "\t3. Thick\n";
    std::cin >> lineThickness;
    return static_cast<figure::ELineThickness>(lineThickness - 1);
}

enum ECommand : uint32_t {
    EXIT,
    ADD_CIRCLE,
    ADD_SQUARE,
    ADD_TRIANGLE,
    ADD_STAR,
    SET_LINE_TYPE,
    SET_LINE_THICKNESS,
    PRINT_FIGURE,
    PRINT_ALL_FIGURES,
    REMOVE_FIGURE,
    REMOVE_ALL_FIGURES
};

int main() {
    using namespace figure;

    uint32_t idCounter = 0;
    std::unordered_map<uint32_t, TSmartFigure> idToFigure;
    while(true) {
        std::cout << "Enter a command:\n"
                  << "\t1. Exit\n"
                  << "\t2. Add circle\n"
                  << "\t3. Add square\n"
                  << "\t4. Add triangle\n"
                  << "\t5. Add star\n"
                  << "\t6. Set line type\n"
                  << "\t7. Set line thickness\n"
                  << "\t8. Print figure\n"
                  << "\t9. Print all figures\n"
                  << "\t10. Remove figure\n"
                  << "\t11. Remove all figures\n";
        uint32_t commandNum;
        std::cin >> commandNum;

        std::cout << '\n';

        // TODO Заменить switch на отдельный класс CommandPerformer
        switch(static_cast<ECommand>(commandNum - 1)) {
            case ECommand::EXIT: {
                return 0;
            }
            case ECommand::ADD_CIRCLE: {
                TPoint point = GetCoordinates();
                idToFigure.emplace(idCounter, new TCircle(idCounter, std::move(point)));
                ++idCounter;
                break;
            }
            case ECommand::ADD_SQUARE: {
                TPoint point = GetCoordinates();
                idToFigure.emplace(idCounter, new TSquare(idCounter, std::move(point)));
                ++idCounter;
                break;
            }
            case ECommand::ADD_TRIANGLE: {
                TPoint point = GetCoordinates();
                idToFigure.emplace(idCounter, new TTriangle(idCounter, std::move(point)));
                ++idCounter;
                break;
            }
            case ECommand::ADD_STAR: {
                TPoint point = GetCoordinates();
                idToFigure.emplace(idCounter, new TStar(idCounter, std::move(point)));
                ++idCounter;
                break;
            }
            case ECommand::SET_LINE_TYPE: {
                const uint32_t id = GetFigureID();
                if (idToFigure.count(id)) {
                    idToFigure[id].SetLineType(GetLineType())
                                  .Print();
                }
                break;
            }
            case ECommand::SET_LINE_THICKNESS: {
                const uint32_t id = GetFigureID();
                if (idToFigure.count(id)) {
                    idToFigure[id].SetLineThickness(GetLineThickness())
                                  .Print();
                }
                break;
            }
            case ECommand::PRINT_FIGURE: {
                const uint32_t id = GetFigureID();
                if (idToFigure.count(id)) {
                    idToFigure[id].Print();
                }
                break;
            }
            case ECommand::PRINT_ALL_FIGURES: {
                if (!idToFigure.size()) {
                    std::cout << "No figures yet!\n";
                } else {
                    for (auto& [_, figure] : idToFigure) {
                        figure.Print();
                    }
                }
                break;
            }
            case ECommand::REMOVE_FIGURE: {
                const uint32_t id = GetFigureID();
                idToFigure.erase(id);
                break;
            }
            case ECommand::REMOVE_ALL_FIGURES: {
                idToFigure.clear();
                break;
            }
            default: {
                std::cout << "Unknown command, try again.\n";
                break;
            }
        }

        std::cout << '\n';
    }
}
