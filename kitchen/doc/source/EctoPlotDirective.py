import sys, shlex, os, re
from os import path

from docutils import nodes
from docutils.parsers import rst
from docutils.parsers.rst.directives import flag, unchanged

from sphinx import addnodes
from sphinx.util.osutil import ensuredir
from sphinx.ext.graphviz import graphviz as graphviz_node
import ecto

class ectoplot(nodes.Element):
    pass

class EctoPlotDirective(rst.Directive):
    has_content = False
    final_argument_whitespace = True
    required_arguments = 2

    #option_spec = dict(shell=flag, prompt=flag, nostderr=flag,
    #                   ellipsis=_slice, extraargs=unchanged)

    def run(self):
        document = self.state.document
        filename = self.arguments[0]
        env = document.settings.env

        if filename.startswith('/') or filename.startswith(os.sep):
            rel_fn = filename[1:]
        else:
            docdir = path.dirname(env.doc2path(env.docname, base=None))
            rel_fn = path.join(docdir, filename)
        try:
            fn = path.join(env.srcdir, rel_fn)
        except UnicodeDecodeError:
            # the source directory is a bytestring with non-ASCII characters;
            # let's try to encode the rel_fn in the file system encoding
            rel_fn = rel_fn.encode(sys.getfilesystemencoding())
            fn = path.join(env.srcdir, rel_fn)

        node = ectoplot()
        node.filename = fn
        node.plasmname = self.arguments[1]
        return [node]

def do_ectoplot(app, doctree):

    def full_executable_path(invoked, path):
        
        import os
        if invoked[0] == '/':
            return invoked

        for d in path:
            full_path = os.path.join(d, invoked)
            if os.path.exists( full_path ):
                return full_path

        return invoked # Not found; invoking it will likely fail

    for node in doctree.traverse(ectoplot):

        l = {}

        execfile(node.filename, globals(), l)

        dottxt = l[node.plasmname].viz()
        
        # NOT ELEGANT!  HACK!
        # clean up message type names a bit
        # there should be some standard way of making type names readable,
        # and the graph shouldn't output such nastiness.
        cre = r'boost::shared_ptr&lt;([A-Za-z0-9_]+::[A-Za-z0-9]+)_&lt;std::allocator&lt;void&gt; &gt; const&gt;'
        dottxt = re.sub(cre, r'\1::ConstPtr', dottxt)

        n = graphviz_node()
        n['code'] = dottxt
        n['options'] = [] #'-Gsize=3', '-Gdpi=68' #hack for now as svg not working.
        node.replace_self(n)

def setup(app):
    app.add_directive('ectoplot', EctoPlotDirective)
    app.connect('doctree-read', do_ectoplot)

