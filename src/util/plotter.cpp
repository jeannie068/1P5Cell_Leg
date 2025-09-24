#include "const.hpp"
#include "plotter.hpp"

using namespace std;

Plotter::Plotter()
    : title_(UNDEFINED_STRING),
      line_styles_(),
      lines_by_style_id_(),
      arrow_styles_(),
      arrows_by_style_id_(),
      filled_rects_by_style_id_(),
      filled_rect_border_styles_() {
}

Plotter::Plotter(const string& title)
    : title_(title),
      line_styles_(),
      lines_by_style_id_(),
      arrow_styles_(),
      arrows_by_style_id_(),
      filled_rects_by_style_id_(),
      filled_rect_border_styles_() {
}

void Plotter::Plot(ostream& os) const {
  os << "set title '" << title_ << "'" << endl;
  os << endl;

  // os << "set output 'output.png'" << endl;

  for (int i = 0; i < line_styles_.size(); ++i) {
    os << "set style line " << i + 1 << " " << line_styles_[i] << endl;
  }
  os << endl;

  for (int i = 0; i < arrow_styles_.size(); ++i) {
    os << "set style arrow " << i + 1 << " " << arrow_styles_[i] << endl;
  }
  os << endl;

  // os << "set size ratio -1" << endl;
  os << "set clip two" << endl;
  os << "unset key" << endl;
  os << endl;

  // 在 plot 指令之前設定全域的填充樣式
  os << "set style fill solid 0.5" << endl;
  os << endl;
  os << "plot ";

  // 先輸出填充矩形
  for (int i = 0; i < filled_rects_by_style_id_.size(); ++i) {
    if (!filled_rects_by_style_id_[i].empty()) {
      os << "'-' w filledcurves closed ls " << i + 1 << ", ";
    }
  }

  // 最後輸出其他線條
  for (int i = 0; i < lines_by_style_id_.size(); ++i) {
      os << "'-' w l ls " << i + 1 << ", ";
  }

  // 輸出填充矩形的邊框（畫在最上層）
  for (int i = 0; i < filled_rects_by_style_id_.size(); ++i) {
      if (!filled_rects_by_style_id_[i].empty()) {
          os << "'-' w l ls " << filled_rect_border_styles_[i] + 1 << ", ";
      }
  }
  
  // arrows 保持不變
  for (int i = 0; i < arrows_by_style_id_.size(); ++i) {
      os << "'-' w vectors arrowstyle " << i + 1 << ", ";
  }
  os << endl;

  // 輸出填充矩形的資料
  for (int i = 0; i < filled_rects_by_style_id_.size(); ++i) {
    if (!filled_rects_by_style_id_[i].empty()) {
      os << endl;
      for (const Rect& rect : filled_rects_by_style_id_[i]) {
        // 輸出閉合的矩形路徑供 filledcurves 使用
        os << rect.min_corner().x() << " " << rect.min_corner().y() << endl;
        os << rect.min_corner().x() << " " << rect.max_corner().y() << endl;
        os << rect.max_corner().x() << " " << rect.max_corner().y() << endl;
        os << rect.max_corner().x() << " " << rect.min_corner().y() << endl;
        os << rect.min_corner().x() << " " << rect.min_corner().y() << endl;
        os << endl;
      }
      os << "e" << endl;
      os << endl;
    }
  }

  // 輸出一般線條的資料
  for (int i = 0; i < lines_by_style_id_.size(); ++i) {
    os << endl;
    for (const Line& line : lines_by_style_id_[i]) {
      os << line.first.x() << " " << line.first.y() << endl;
      os << line.second.x() << " " << line.second.y() << endl;
      os << endl;
    }
    os << "e" << endl;
    os << endl;
  }
  os << endl;

  // 輸出填充矩形邊框的資料
  for (int i = 0; i < filled_rects_by_style_id_.size(); ++i) {
    if (!filled_rects_by_style_id_[i].empty()) {
      os << endl;
      for (const Rect& rect : filled_rects_by_style_id_[i]) {
        // 輸出矩形邊框
        os << rect.min_corner().x() << " " << rect.min_corner().y() << endl;
        os << rect.min_corner().x() << " " << rect.max_corner().y() << endl;
        os << endl;
        os << rect.min_corner().x() << " " << rect.max_corner().y() << endl;
        os << rect.max_corner().x() << " " << rect.max_corner().y() << endl;
        os << endl;
        os << rect.max_corner().x() << " " << rect.max_corner().y() << endl;
        os << rect.max_corner().x() << " " << rect.min_corner().y() << endl;
        os << endl;
        os << rect.max_corner().x() << " " << rect.min_corner().y() << endl;
        os << rect.min_corner().x() << " " << rect.min_corner().y() << endl;
        os << endl;
      }
      os << "e" << endl;
      os << endl;
    }
  }
  os << endl;

  // 輸出箭頭的資料
  for (int i = 0; i < arrows_by_style_id_.size(); ++i) {
    os << endl;
    for (const Line& arrow : arrows_by_style_id_[i]) {
      os << arrow.first.x() << " " << arrow.first.y() << " "
         << arrow.second.x() - arrow.first.x() << " "
         << arrow.second.y() - arrow.first.y() << endl;
    }
    os << "e" << endl;
    os << endl;
  }
  os << endl;

  os << "pause -1 'Press enter to exit.'" << endl;
}

// Setters

LineStyleId Plotter::add_line_style(const string& style) {
  const LineStyleId line_style_id(line_styles_.size());

  line_styles_.push_back(style);
  lines_by_style_id_.push_back(vector<Line>());

  return line_style_id;
}

ArrowStyleId Plotter::add_arrow_style(const string& style) {
  const ArrowStyleId arrow_style_id(arrow_styles_.size());

  arrow_styles_.push_back(style);
  arrows_by_style_id_.push_back(vector<Line>());

  return arrow_style_id;
}

void Plotter::add_line(const Line& line, LineStyleId line_style_id) {
  lines_by_style_id_.at(line_style_id).push_back(line);
}

void Plotter::add_rect(const Rect& rect, LineStyleId line_style_id) {
  lines_by_style_id_.at(line_style_id)
      .push_back(Line(Point(rect.min_corner().x(), rect.min_corner().y()),
                      Point(rect.min_corner().x(), rect.max_corner().y())));
  lines_by_style_id_.at(line_style_id)
      .push_back(Line(Point(rect.min_corner().x(), rect.max_corner().y()),
                      Point(rect.max_corner().x(), rect.max_corner().y())));
  lines_by_style_id_.at(line_style_id)
      .push_back(Line(Point(rect.max_corner().x(), rect.max_corner().y()),
                      Point(rect.max_corner().x(), rect.min_corner().y())));
  lines_by_style_id_.at(line_style_id)
      .push_back(Line(Point(rect.max_corner().x(), rect.min_corner().y()),
                      Point(rect.min_corner().x(), rect.min_corner().y())));
}

void Plotter::add_polygon(const Polygon& polygon, LineStyleId line_style_id) {
  bool is_first_point = true;
  Point last_point;

  for (const Point& point : polygon.outer()) {
    if (!is_first_point) {
      lines_by_style_id_.at(line_style_id).push_back(Line(last_point, point));
    }

    last_point = point;

    if (is_first_point) {
      is_first_point = false;
    }
  }

  for (const auto& ring : polygon.inners()) {
    is_first_point = true;

    for (const Point& point : ring) {
      if (!is_first_point) {
        lines_by_style_id_.at(line_style_id).push_back(Line(last_point, point));
      }

      last_point = point;

      if (is_first_point) {
        is_first_point = false;
      }
    }
  }
}

void Plotter::add_filled_rect(const Rect& rect, LineStyleId fill_style_id, LineStyleId border_style_id) {
  // 確保 filled_rects_by_style_id_ 有足夠大小
  if (fill_style_id > filled_rects_by_style_id_.size() || 
      fill_style_id == filled_rects_by_style_id_.size()) {
    filled_rects_by_style_id_.resize(fill_style_id + 1);
    filled_rect_border_styles_.resize(fill_style_id + 1);
  }
  
  filled_rects_by_style_id_[fill_style_id].push_back(rect);
  filled_rect_border_styles_[fill_style_id] = border_style_id;
}

/*
void Plotter::add_filled_rect(const Rect& rect, LineStyleId line_style_id) {
  // 使用 gnuplot 的 boxes 物件來繪製填充矩形
  // 這需要特殊處理，因為 gnuplot 的 boxes 需要中心點和寬高
  // 或者使用 filledcurves 閉合路徑
  lines_by_style_id_.at(line_style_id)
      .push_back(Line(Point(rect.min_corner().x(), rect.min_corner().y()),
                      Point(rect.min_corner().x(), rect.max_corner().y())));
  lines_by_style_id_.at(line_style_id)
      .push_back(Line(Point(rect.min_corner().x(), rect.max_corner().y()),
                      Point(rect.max_corner().x(), rect.max_corner().y())));
  lines_by_style_id_.at(line_style_id)
      .push_back(Line(Point(rect.max_corner().x(), rect.max_corner().y()),
                      Point(rect.max_corner().x(), rect.min_corner().y())));
  lines_by_style_id_.at(line_style_id)
      .push_back(Line(Point(rect.max_corner().x(), rect.min_corner().y()),
                      Point(rect.min_corner().x(), rect.min_corner().y())));
  // 再次連接到起點以形成閉合路徑
  lines_by_style_id_.at(line_style_id)
      .push_back(Line(Point(rect.min_corner().x(), rect.min_corner().y()),
                      Point(rect.min_corner().x(), rect.min_corner().y())));
}*/

void Plotter::add_arrow(const Line& arrow, ArrowStyleId arrow_style_id) {
  arrows_by_style_id_.at(arrow_style_id).push_back(arrow);
}
