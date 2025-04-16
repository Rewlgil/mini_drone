from http.server import BaseHTTPRequestHandler, HTTPServer
import os
import json
from urllib.parse import unquote

class MyHandler(BaseHTTPRequestHandler):
    def _set_headers(self, content_type='text/html'):
        self.send_response(200)
        self.send_header('Content-type', content_type)
        self.send_header('Access-Control-Allow-Origin', '*')
        self.send_header('Access-Control-Allow-Methods', 'GET, POST, OPTIONS')
        self.send_header('Access-Control-Allow-Headers', 'Content-Type')
        self.end_headers()

    def do_OPTIONS(self):
        self._set_headers()

    def do_GET(self):
        path = unquote(self.path)
        if path == '/':
            path = '/index.html'

        file_path = './data' + path
        ext = os.path.splitext(file_path)[1]
        content_type = {
            '.html': 'text/html',
            '.js': 'application/javascript',
            '.css': 'text/css',
            '.png': 'image/png',
            '.jpg': 'image/jpeg',
            '.gif': 'image/gif'
        }.get(ext, 'application/octet-stream')

        try:
            with open(file_path, 'rb') as f:
                content = f.read()
            self._set_headers(content_type)
            self.wfile.write(content)
        except FileNotFoundError:
            self.send_error(404, f"File not found: {file_path}")

    def do_POST(self):
        content_length = int(self.headers['Content-Length'])
        post_data = self.rfile.read(content_length)

        try:
            data = json.loads(post_data)
            print("Received data:", data)
        except json.JSONDecodeError:
            print("Received invalid JSON:", post_data.decode('utf-8'))

        self._set_headers('text/plain')
        self.wfile.write(b'Data received!')

def main(port=8000):
    server_address = ('', port)
    httpd = HTTPServer(server_address, MyHandler)
    print(f"Serving files and accepting POST on http://localhost:{port}")
    httpd.serve_forever()

if __name__ == '__main__':
    main()
