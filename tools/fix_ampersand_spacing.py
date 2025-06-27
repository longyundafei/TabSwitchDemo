# -*- coding: utf-8 -*-
import re
import sys

def fix_amp_spacing(xml_content):
    # 只处理<string ...>...</string>中的内容
    def fix_line(match):
        name = match.group(1)
        value = match.group(2)

        # 修复 &amp;后紧跟非空格（比如中文或字母） 的情况，加一个空格
        fixed_value = re.sub(r'&amp;(?=\S)', '&amp; ', value)

        return u'<string name="%s">%s</string>' % (name, fixed_value)

    # 匹配每一行的 <string name="...">...</string>
    pattern = re.compile(ur'<string name="([^"]+)">(.*?)</string>', re.UNICODE)

    fixed_content = pattern.sub(fix_line, xml_content)

    return fixed_content


if __name__ == '__main__':
    if len(sys.argv) != 3:
        print("Usage: python fix_ampersand_spacing.py input.xml output.xml")
        sys.exit(1)

    input_path = sys.argv[1]
    output_path = sys.argv[2]

    with open(input_path, 'r') as f:
        raw = f.read().decode('utf-8')

    fixed = fix_amp_spacing(raw)

    with open(output_path, 'w') as f:
        f.write(fixed.encode('utf-8'))

    print("Fix completed. Output saved to: %s" % output_path)
